/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 Metrological
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include "Module.h"

// @stubgen:include <com/IIteratorType.h>

namespace Thunder {
namespace Exchange {

    // @json
    struct EXTERNAL ILANControl : virtual public Core::IUnknown {
        enum { ID = ID_LANCONTROL };
        
        enum StatusType : uint8_t {
            DOWN,
            UP
        };

        struct Config {
            string bridge_name;
            string bridge_mac;
            string bridge_ipv4;
            string bridge_ipv6;
            string ipv4_range_start;
            string ipv4_range_end;
            string ipv6_range_start;
            string ipv6_range_end;
            string* dnsNameservers;
            string mac_address;
            string ip_address;
        };

        using IStringIterator = RPC::IIteratorType<string, RPC::ID_STRINGITERATOR>;

        // @event
        struct EXTERNAL INotification : virtual public Core::IUnknown {
            enum { ID = ID_LANCONTROL_NOTIFICATION };
            ~INotification() override = default;

            // @brief Signal network change
            // @param network: Name of the network that has changed
            // @param bridgeName: Name of the linux bridge interface used for this network
            // @param status: Status of the linux bridge interface
            virtual void NetworkChange(const string& network, const string& bridgeName, const StatusType& status) = 0;
        };

         ~ILANControl() override = default;

        // Pushing notifications to interested sinks
        virtual uint32_t Register(ILANControl::INotification* sink) = 0;
        virtual uint32_t Unregister(ILANControl::INotification* sink) = 0;

        // @property
        // @brief Currently available networks
        virtual uint32_t Networks(IStringIterator*& networks /* @out */) const = 0;

        // @property
        // @brief Status of requested network
        virtual uint32_t Status(const string& network /* @index */, string& bridgeName /* @out */, StatusType& status /* @out */) const = 0;

        // @property
        // @brief Configuration of requested network
        // @param configInfo: Configuration info of requested network
        // @retval ERROR_UNAVAILABLE Failed to set/retrieve config
        virtual uint32_t Config(const string& network /* @index */, Config& config /* @out */) const = 0;
        virtual uint32_t Config(const string& network /* @index */, const Config& config /* @in */) = 0;

        // @property
        // @brief Network up or down
        // @param up: Up/Down given network
        // @retval ERROR_UNAVAILABLE Failed to set/retrieve UP
        virtual uint32_t Up(const string& network /* @index */, bool& up /* @out */) const = 0;
        virtual uint32_t Up(const string& network /* @index */, const bool up /* @in */) = 0;

        // @brief Reset requested network
        // @param interface: Name of the network to be reset
        virtual uint32_t Reset(const string& network) = 0;
    };

} // namespace Exchange
} // namespace Thunder