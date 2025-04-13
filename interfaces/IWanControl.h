/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2025 Sky
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

    /* @json */
    struct EXTERNAL IWANControl : virtual public Core::IUnknown {
        enum { ID = ID_WANCONTROL };

        enum StatusType : uint8_t {
            DOWN,
            UP
        };

        enum ActiveStatus : uint8_t {
            INACTIVE,
            ACTIVE
        };

        struct Dhcpv4Info {
            uint8_t       addressAssigned;              /** Have we been assigned an IP address ? */
            uint8_t       isExpired;                    /** Is the lease time expired ? */
            string        ip;                           /** New IP address, if addressAssigned==TRUE */
            string        netmask;                      /** New netmask, if addressAssigned==TRUE */
            string        gateway;                      /** New gateway, if addressAssigned==TRUE */
            string        dnsServers;                   /** New dns Servers, if addressAssigned==TRUE */
            string        dhcpcInterface;               /** Dhcp interface name */
            uint32_t      leaseTime;                    /** Lease time, , if addressAssigned==TRUE */
            uint32_t      rebindingTime;                /** Rebinding time, if addressAssigned==TRUE */
            uint32_t      renewalTime;                  /** Renewal Time, if addressAssigned==TRUE */
            int32_t       timeOffset;                   /** New time offset, if addressAssigned==TRUE */
            uint8_t       isTimeOffsetAssigned;         /** Is the time offset assigned ? */
            string        timeZone;                     /** New time zone, if addressAssigned==TRUE */
            uint8_t       mtuAssigned;                  /** Have we been assigned MTU size ? */
            uint16_t      mtuSize;                      /** MTU Size, if mtuAssigned==TRUE */
        };

        struct Dhcpv6Info
        {
            uint8_t       prefixAssigned;               /**< Have we been assigned a site prefix ? */
            uint8_t       addrAssigned;                 /**< Have we been assigned an IPv6 address ? */
            uint8_t       dnsAssigned;                  /**< Have we been assigned dns server addresses ? */
            uint8_t       domainNameAssigned;           /**< Have we been assigned domain name ? */
            uint8_t       aftrAssigned;                 /**< Have we been assigned aftr name ? */
            uint8_t       mapeAssigned;                 /**< Have we been assigned mape config ? */
            uint8_t       maptAssigned;                 /**< Have we been assigned mapt config ? */
            uint8_t       isExpired;                    /**< Is the lease time expired ? */
            string        sitePrefix;                   /**< New site prefix, if prefixAssigned==TRUE */
            uint32_t      prefixPltime;                 /**< IPv6 Prefix Preferred Life Time */
            uint32_t      prefixVltime;                 /**< IPv6 Prefix Valid Life Time */
            string        sitePrefixOld;                /**< add support for RFC7084 requirement L-13 */
            uint32_t      prefixVltimeOld;              /**< add support for RFC7084 requirement L-13 */
            uint32_t      prefixCmd;
            string        ifname;
            string        address;                      /**< New IPv6 address, if addrAssigned==TRUE */
            uint32_t      addressPltime;                /**< New IPv6 address, Preferred Life Time */
            uint32_t      addressVltime;                /**< New IPv6 address, Valid Life Time */
            string        pdIfAddress;                  /**< New IPv6 address of PD interface */
            uint32_t      addrCmd;
            string        nameservers;                  /**< New nameserver, if addressAssigned==TRUE */
            string        domainName;                   /**< New domain Name, if addressAssigned==TRUE */
            string        ntpserver;                    /**< New ntp server(s), dhcp server may provide this */
        };

        struct VirtualInterfaceConfig {
            string      alias                                /* @brief Alias Name of Interface Name */;
            string      virtualIfaceName                     /* @brief L3(Virtual) Interface Name */;
        };

        struct NetworkInterfaceStatus {
            String       alias                                 /* @brief current wan interface name */;
            ActiveStatus active_status                         /* @brief WAN interface active status */;
        };

        struct NetworkStatusInfo {
            String                      current_wan_interface_name              /* @brief current wan interface name */;
            StatusType                  wan_status                              /* @brief WAN status */;
            NetworkInterfaceStatus      *pstAvailable_Wan_Interfaces_Status     /* @brief available wan interface status */;
        };

        struct NetworkInfo {
            struct NetworkStatusInfo    statusInfo           /* @brief Network Status Information */;
            struct Dhcpv4Info           dhcpv4Info           /* @brief Network IPv4 Information */;
            struct Dhcpv6Info           dhcpv6Info           /* @brief Network IPv6 Information */;
        };

        using IStringIterator = RPC::IIteratorType<string, RPC::ID_STRINGITERATOR>;

        // @event
        struct EXTERNAL INotification : virtual public Core::IUnknown {
            enum { ID = ID_WANCONTROL_NOTIFICATION };

            ~INotification() override = default;

            // @brief Signal WAN network status update
            // @param networkInfo: Active WAN network information where an update occured
            // @retval ERROR_UNAVAILABLE Failed to update WAN network information
            virtual uint32_t UpdateOnWANInfoChange(const NetworkStatusInfo& networkInfo) = 0;
        };

        ~IWANControl() override = default;

        // Pushing notifications to interested sinks
        virtual uint32_t Register(IWANControl::INotification* sink) = 0;
        virtual uint32_t Unregister(IWANControl::INotification* sink) = 0;

        // @property
        // @brief Get WAN network information
        // @param networkInfo: Output of WAN Information including Active, Available WAN status
        // @retval ERROR_UNAVAILABLE Failed to retrieve WAN network information
        virtual uint32_t GetNetworkInfo(NetworkInfo& networkInfo /* @out */) const = 0;

        // @property
        // @brief Create WAN Interface
        // @param baseInterface: Input parameter for passing Base Interface Name to 
        //                       use a lookup to create virtual interface on top of this interface
        // @param virtualIfaceInfo: Input of Virtual Interface Information
        // @retval ERROR_UNAVAILABLE Failed to create WAN Virtual Interface
        virtual uint32_t CreateInterface(const string& baseInterface /* @in */, const struct VirtualInterfaceConfig virtualIfaceInfo /* @in */) = 0;

        // @property
        // @brief Delete WAN Interface
        // @param alias: Input parameter for pasing alias to lookup corresponding virtual 
        //               interface to be delete and detach from base interface
        // @retval ERROR_UNAVAILABLE Failed to delete WAN Interface
        virtual uint32_t DeleteInterface(const string& alias /* @in */) = 0;

        // @property
        // @brief Get WAN Interfaces Information
        // @param interfaces: returns list of alias information with respect to base interface and virtual interface
        // @retval ERROR_UNAVAILABLE Failed to retrieve WAN Interfaces
        virtual uint32_t GetInterfaces(IStringIterator*& interfaces /* @out */) const = 0;
        
        // @property
        // @brief Activate WAN Interface
        // @param virtualInterface: interface to be Activate
        // @retval ERROR_UNAVAILABLE Failed to retrieve WAN Interface
        virtual uint32_t ActivateInterface(const string& virtualInterface /* @in */) = 0;
    };
}
}
