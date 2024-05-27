#include "router.hh"

#include <iostream>
#include <limits>
#include <queue>

using namespace std;

// route_prefix: The "up-to-32-bit" IPv4 address prefix to match the datagram's destination address against
// prefix_length: For this route to be applicable, how many high-order (most-significant) bits of
//    the route_prefix will need to match the corresponding bits of the datagram's destination address?
// next_hop: The IP address of the next hop. Will be empty if the network is directly attached to the router (in
//    which case, the next hop address should be the datagram's final destination).
// interface_num: The index of the interface to send the datagram out on.
void Router::add_route( const uint32_t route_prefix,
                        const uint8_t prefix_length,
                        const optional<Address> next_hop,
                        const size_t interface_num )
{
  cerr << "DEBUG: adding route " << Address::from_ipv4_numeric( route_prefix ).ip() << "/"
       << static_cast<int>( prefix_length ) << " => " << ( next_hop.has_value() ? next_hop->ip() : "(direct)" )
       << " on interface " << interface_num << "\n";

  // Your code here.
  routes_.add( route_prefix, prefix_length, next_hop, interface_num );
}

// Go through all the interfaces, and route every incoming datagram to its proper outgoing interface.
void Router::route()
{
  // Your code here.
  for ( auto& interface : _interfaces ) {
    queue<InternetDatagram>& q = interface->datagrams_received();
    while ( !q.empty() ) {
      InternetDatagram& dgram = q.front();
      uint32_t ip_num = dgram.header.dst;
      pair<optional<Address>, size_t> action;
      if ( dgram.header.ttl > 1 && routes_.find( ip_num, action ) ) {
        --dgram.header.ttl;
        dgram.header.compute_checksum();
        if ( action.first.has_value() ) {
          _interfaces[action.second]->send_datagram( dgram, action.first.value() );
        } else {
          _interfaces[action.second]->send_datagram( dgram, Address::from_ipv4_numeric( ip_num ) );
        }
      }
      q.pop();
    }
  }
}
