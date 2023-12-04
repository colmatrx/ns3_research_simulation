/* Author - Idris Adeleke
   Title - Prototype 1
   
   This module creates a tolpology as shown and n0 serves as the client
   node n1 is skipped but used in promiscuous mode to capture network traffic
   nodes n2,n3 and n4 are used as service provider servers with which the client n0 interacts
   
   The simulation runs for 30 seconds with node n0 echoing a total of 30 packets - 1 packet of data every 1 second to nodes n2, n3 and n4
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

// Network Topology
//
//       10.1.1.0
// n0 -------------- n1   n2   n3   n4
//    point-to-point  |    |    |    |
//                    ================
//                      LAN 10.1.2.0


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("prototype1");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nserviceProviderServer = 3;	//number of servers that the client is talking to at a time

  CommandLine cmd (__FILE__);
  cmd.AddValue ("nproviderServer", "Number of \"extra\" CSMA nodes/devices", nserviceProviderServer);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_ALL);   //maximum log level to log everything; the logging is redirected to logfile.log at run time from the cmd line
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_ALL);   //maximum log level to log everything
    }

  nserviceProviderServer = nserviceProviderServer == 0 ? 1 : nserviceProviderServer;

  NodeContainer clientServerp2pNodes;       //instantiates NodeContainer for the point to point segment of the topology
  clientServerp2pNodes.Create (2);          //calls the create method to createtwo p2p node part of the topology

  NodeContainer serviceProviderServerNodes;      //instantiates container for ethernet (CSMA) part of the topology where the servers are
  serviceProviderServerNodes.Add (clientServerp2pNodes.Get (1));     //calls the add method to add one p2p node to the CSMA container. This is the p2p node on the side of the other CSMA nodes as shown in the topology
  serviceProviderServerNodes.Create (nserviceProviderServer);     //creates the number of CSMA nodes

  PointToPointHelper p2pClientServerHelper;      //instantiates p2p helper object to set up the point to point segment of the topology
  p2pClientServerHelper.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));  //sets the data rate attribute of the p2p object
  p2pClientServerHelper.SetChannelAttribute ("Delay", StringValue ("2ms"));      //sets the link/channel delay on the p2p medium

  NetDeviceContainer p2pclientServerDevices;        // create p2p network card/devices objects
  p2pclientServerDevices = p2pClientServerHelper.Install (clientServerp2pNodes); //installs the p2p network cards above on the p2p nodes

  CsmaHelper serviceProviderServerHelper;      //creates a CSMA helper object to setup the ethernet channel/medium
  serviceProviderServerHelper.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));       //sets the CSMA medium data rate
  serviceProviderServerHelper.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));   //sets the CSMA medium delay

  NetDeviceContainer serviceProviderServerDevices;       //creates CSMA network cards/devices
  serviceProviderServerDevices = serviceProviderServerHelper.Install (serviceProviderServerNodes);       //intsalls the CSMA cards above onto the CSMA nodes

  InternetStackHelper ipProtocolStack;    //instantiates the InternetStackHelper for enabling the ip protocol
  ipProtocolStack.Install (clientServerp2pNodes.Get (0));     //install the protocol stack on the client side p2p node
  ipProtocolStack.Install (serviceProviderServerNodes);            //install the protocol stack on the server side p2p node and the rest of the servers on the LAN (CSMA) medium

  Ipv4AddressHelper ipAddress;            //creates the Ipv4AddressHelper object named ipAddress
  ipAddress.SetBase ("10.1.1.0", "255.255.255.0");        //calls the setbase method to initiate the netwrok address and subnet mask of the p2p subnet
  Ipv4InterfaceContainer clientServerp2pInterfaces;                 //creates a container to hold the IP addresses of the p2p devices
  clientServerp2pInterfaces = ipAddress.Assign (p2pclientServerDevices);          //assign IP addresses to the p2p devices earlier created

  ipAddress.SetBase ("10.1.2.0", "255.255.255.0");        //calls the setbase method to initiate the netwrok address and subnet mask of the CSMA subnet       
  Ipv4InterfaceContainer serviceProviderCsmaInterfaces;                //creates a container to hold the IP addresses of the CSMA devices
  serviceProviderCsmaInterfaces = ipAddress.Assign (serviceProviderServerDevices);        //assign IP addresses to the CSMA devices earlier created

  UdpEchoServerHelper echoServer (9);   //creates an echoServer application with the port number 9 as constructor parameter

 // ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma)); //install the echoServer application on the last CSMA device on the LAN
  ApplicationContainer serverApps = echoServer.Install (serviceProviderServerNodes); //install echoServer app on all CSMA nodes
  serverApps.Start (Seconds (1.0));     //CSMA servers start listening at 1.0second
  serverApps.Stop (Seconds (30.0));     //CSMA servers stop listening at 10.0second

  //UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 9);        //creates cleint application and provides the remote (server) address to which the echoCLient talks
  UdpEchoClientHelper echoClient1 (serviceProviderCsmaInterfaces.GetAddress (1), 9);  //creates cleint application and provides the remote (server) address to which the echoCLient1 talks
  UdpEchoClientHelper echoClient2 (serviceProviderCsmaInterfaces.GetAddress (2), 9);  //creates cleint application and provides the remote (server) address to which the echoCLient2 talks
  UdpEchoClientHelper echoClient3 (serviceProviderCsmaInterfaces.GetAddress (3), 9);  //creates cleint application and provides the remote (server) address to which the echoCLient3 talks 
  //UdpEchoClientHelper echoClientnCsma (csmaInterfaces.GetAddress (nCsma), 9);    
  
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (30));            //number of packets sent at once by the client during simulation
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));      //transmission interval between packets sent
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));         //size of each packet sent by the client

  echoClient2.SetAttribute ("MaxPackets", UintegerValue (30));            //number of packets sent at once by the client
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));      //transmission interval between packets sent
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));         //size of each packet sent by the client
  
  echoClient3.SetAttribute ("MaxPackets", UintegerValue (30));            //number of packets sent at once by the client
  echoClient3.SetAttribute ("Interval", TimeValue (Seconds (1.0)));      //transmission interval between packets sent
  echoClient3.SetAttribute ("PacketSize", UintegerValue (1024));         //size of each packet sent by the client
  
  ApplicationContainer clientApps1 = echoClient1.Install (clientServerp2pNodes.Get (0));      //install the client application on the client
  ApplicationContainer clientApps2 = echoClient2.Install (clientServerp2pNodes.Get (0));      //install the client application on the client
  ApplicationContainer clientApps3 = echoClient3.Install (clientServerp2pNodes.Get (0));      //install the client application on the client
  
  clientApps1.Start (Seconds (2.0));     //client starts transmission at 2.0seconds
  clientApps1.Stop (Seconds (30.0));     //cleint stops transmission at 10.0seconds

  clientApps2.Start (Seconds (2.0));     //client starts transmission at 2.0seconds
  clientApps2.Stop (Seconds (30.0));     //cleint stops transmission at 10.0seconds
  
  clientApps3.Start (Seconds (2.0));     //client starts transmission at 2.0seconds
  clientApps3.Stop (Seconds (30.0));     //cleint stops transmission at 10.0seconds
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();            //setup global routing to manage routing of the internetwork

  p2pClientServerHelper.EnablePcapAll ("prototype1");              //enables us to capture the point to point trace of both p2p nodes
  serviceProviderServerHelper.EnablePcap ("prototype1", serviceProviderServerDevices.Get (0), true);      //enable packet capture in promiscuous node to sniff the entire network traffic
 
 AsciiTraceHelper ascii; //for asci based tracing
 p2pClientServerHelper.EnableAsciiAll (ascii.CreateFileStream ("p2pprototype1.tr")); //for tracing only p2p traffic
 serviceProviderServerHelper.EnableAsciiAll (ascii.CreateFileStream ("csmaprototype1.tr")); //for tracing only CSMA traffic
 

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
