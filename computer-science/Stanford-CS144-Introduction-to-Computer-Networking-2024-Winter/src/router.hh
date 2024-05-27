#pragma once

#include <memory>
#include <optional>
#include <utility>

#include "exception.hh"
#include "network_interface.hh"

class Trie
{
public:
  Trie() : root_( std::make_shared<Node>() ) {}

  void add( uint32_t route_prefix, uint8_t prefix_length, std::optional<Address> next_hop, size_t interface_num )
  {
    if ( prefix_length > 32 )
      return;
    uint32_t curr_mask = 0x80000000;
    std::shared_ptr<Node> curr = root_;
    while ( prefix_length > 0 ) {
      int index = ( route_prefix & curr_mask ) > 0;
      if ( !curr->next[index] ) {
        curr->next[index] = std::make_shared<Node>();
      }
      curr = curr->next[index];
      curr_mask = curr_mask >> 1;
      --prefix_length;
    }
    curr->next_hop = next_hop;
    curr->interface_num = interface_num;
  }

  bool find( uint32_t ip_num, std::pair<std::optional<Address>, size_t>& action )
  {
    uint32_t curr_mask = 0x80000000;
    std::shared_ptr<Node> curr = root_;
    bool found = false;
    while ( curr ) {
      if ( curr->interface_num.has_value() ) {
        action.first = curr->next_hop;
        action.second = curr->interface_num.value();
        found = true;
      }
      int index = ( ip_num & curr_mask ) > 0;
      curr = curr->next[index];
      curr_mask = curr_mask >> 1;
    }
    return found;
  }

private:
  class Node
  {
  public:
    std::vector<std::shared_ptr<Node>> next { nullptr, nullptr };
    std::optional<Address> next_hop {};
    std::optional<size_t> interface_num {};
  };
  std::shared_ptr<Node> root_ {};
};

// \brief A router that has multiple network interfaces and
// performs longest-prefix-match routing between them.
class Router
{
public:
  // Add an interface to the router
  // \param[in] interface an already-constructed network interface
  // \returns The index of the interface after it has been added to the router
  size_t add_interface( std::shared_ptr<NetworkInterface> interface )
  {
    _interfaces.push_back( notnull( "add_interface", std::move( interface ) ) );
    return _interfaces.size() - 1;
  }

  // Access an interface by index
  std::shared_ptr<NetworkInterface> interface( const size_t N ) { return _interfaces.at( N ); }

  // Add a route (a forwarding rule)
  void add_route( uint32_t route_prefix,
                  uint8_t prefix_length,
                  std::optional<Address> next_hop,
                  size_t interface_num );

  // Route packets between the interfaces
  void route();

private:
  // The router's collection of network interfaces
  std::vector<std::shared_ptr<NetworkInterface>> _interfaces {};
  Trie routes_ {};
};
