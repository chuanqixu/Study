#include <iostream>
#include <type_traits>

#include "arp_message.hh"
#include "exception.hh"
#include "network_interface.hh"
#include "parser.hh"

using namespace std;

//! \param[in] ethernet_address Ethernet (what ARP calls "hardware") address of the interface
//! \param[in] ip_address IP (what ARP calls "protocol") address of the interface
NetworkInterface::NetworkInterface( string_view name,
                                    shared_ptr<OutputPort> port,
                                    const EthernetAddress& ethernet_address,
                                    const Address& ip_address )
  : name_( name )
  , port_( notnull( "OutputPort", move( port ) ) )
  , ethernet_address_( ethernet_address )
  , ip_address_( ip_address )
{
  cerr << "DEBUG: Network interface has Ethernet address " << to_string( ethernet_address ) << " and IP address "
       << ip_address.ip() << "\n";
}

//! \param[in] dgram the IPv4 datagram to be sent
//! \param[in] next_hop the IP address of the interface to send it to (typically a router or default gateway, but
//! may also be another host if directly connected to the same network as the destination) Note: the Address type
//! can be converted to a uint32_t (raw 32-bit IP address) by using the Address::ipv4_numeric() method.
void NetworkInterface::send_datagram( const InternetDatagram& dgram, const Address& next_hop )
{
  // Your code here.
  uint32_t ip_num = next_hop.ipv4_numeric();
  if ( arp_table_.count( ip_num ) > 0 ) { // if MAC is stored in arp_table_, then directly transmit
    const EthernetFrame ethernet_frame { { arp_table_[ip_num].first, ethernet_address_, EthernetHeader::TYPE_IPv4 },
                                         serialize( dgram ) };
    transmit( ethernet_frame );
  } else { // if MAC is not stored in arp_table_, then first send the ARP request, and keep the datagram in
           // arp_waiting_table_ waiting for the MAC
    if ( arp_waiting_table_.count( ip_num ) == 0 ) {
      // note: must first push into the queue, then send the ARP request!!!
      // in lab 6, transmit() will directly call recv_frame()
      // in the case of A sends ARP request to B, and B instantly calls recv_frame() and sends ARP reply to A,
      // and A instantly calls recv_frame()
      // if first send ARP request then push into the queue, then A sends ARP request to B while the datagram
      // has not been pushed into the queue, so when A calls recv_frame, it updates arp_table_, but cannot send this
      // datagram that incurs the ARP request, and this datagram will never be sent because now arp_table_ has its
      // entry and maintains it for 30 seconds while arp_waiting_table_ is outdated after 5 seconds

      std::queue<std::pair<InternetDatagram, Address>> q;
      arp_waiting_table_.emplace( ip_num, std::make_pair( q, 0 ) );
      arp_waiting_table_[ip_num].first.emplace( std::make_pair( dgram, next_hop ) );
      const ARPMessage arp_message { ARPMessage::TYPE_ETHERNET,
                                     EthernetHeader::TYPE_IPv4,
                                     sizeof( EthernetHeader::src ),
                                     sizeof( IPv4Header::src ),
                                     ARPMessage::OPCODE_REQUEST,
                                     ethernet_address_,
                                     ip_address_.ipv4_numeric(),
                                     {}, // ignore for ARP request
                                     ip_num };
      const EthernetFrame ethernet_frame { { ETHERNET_BROADCAST, ethernet_address_, EthernetHeader::TYPE_ARP },
                                           serialize( arp_message ) };
      transmit( ethernet_frame );
    } else {
      arp_waiting_table_[ip_num].first.emplace( std::make_pair( dgram, next_hop ) );
    }
  }
}

//! \param[in] frame the incoming Ethernet frame
void NetworkInterface::recv_frame( const EthernetFrame& frame )
{
  // Your code here.
  if ( frame.header.dst != ethernet_address_ && frame.header.dst != ETHERNET_BROADCAST )
    return;

  if ( frame.header.type == EthernetHeader::TYPE_IPv4 ) { // if received IP datagram, send to datagrams_received_
    InternetDatagram dgram;
    if ( !parse( dgram, frame.payload ) )
      return;
    datagrams_received_.push( dgram );
  } else if ( frame.header.type == EthernetHeader::TYPE_ARP ) { // if received ARP, save the sender MAC
    ARPMessage arp_message;
    if ( !parse( arp_message, frame.payload ) )
      return;
    if ( arp_message.target_ip_address != ip_address_.ipv4_numeric() )
      return;

    uint32_t ip_num = arp_message.sender_ip_address;
    arp_table_.emplace( ip_num, std::make_pair( arp_message.sender_ethernet_address, 0 ) );

    if ( arp_message.opcode == ARPMessage::OPCODE_REQUEST ) { // if received ARP request, send the MAC
      const ARPMessage arp_reply_message { ARPMessage::TYPE_ETHERNET,
                                           EthernetHeader::TYPE_IPv4,
                                           sizeof( EthernetHeader::src ),
                                           sizeof( IPv4Header::src ),
                                           ARPMessage::OPCODE_REPLY,
                                           ethernet_address_,
                                           ip_address_.ipv4_numeric(),
                                           arp_message.sender_ethernet_address,
                                           ip_num };
      const EthernetFrame ethernet_frame {
        { arp_message.sender_ethernet_address, ethernet_address_, EthernetHeader::TYPE_ARP },
        serialize( arp_reply_message ) };
      transmit( ethernet_frame );
    } else if ( arp_message.opcode == ARPMessage::OPCODE_REPLY
                && arp_message.target_ethernet_address
                     == ethernet_address_ ) { // if received ARP reply, send the corresponding datagrams in
                                              // arp_waiting_table_
      if ( arp_waiting_table_.count( ip_num ) == 0 )
        return;
      auto q = arp_waiting_table_[ip_num].first;
      while ( !q.empty() ) {
        auto& [dgram, next_hop] = q.front();
        send_datagram( dgram, next_hop );
        q.pop();
      }
      arp_waiting_table_.erase( ip_num );
    }
  }
}

template<class T>
void update_table( const size_t ms_since_last_tick, T& table, size_t time_limit )
{
  auto iter = table.begin();
  while ( iter != table.end() ) {
    size_t& t = iter->second.second;
    t += ms_since_last_tick;
    if ( t > time_limit )
      iter = table.erase( iter );
    else
      ++iter;
  }
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void NetworkInterface::tick( const size_t ms_since_last_tick )
{
  // Your code here.
  update_table( ms_since_last_tick, arp_table_, 30000 );
  update_table( ms_since_last_tick, arp_waiting_table_, 5000 );
}
