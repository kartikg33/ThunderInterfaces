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
 
     /* @json 1.0.0 */
     struct EXTERNAL IFirewallControl : virtual public Core::IUnknown {
         enum { ID = ID_FIREWALLCONTROL };
 
         enum ServiceType : uint8_t {
             WAN2LAN,                        /* @brief For traffic flowing from WAN to LAN */
             LAN2WAN,                        /* @brief For traffic flowing from LAN to WAN  */
             WAN2SELF,                       /* @brief For traffic flowing from WAN to local processes */
             SELF2WAN,                       /* @brief For traffic flowing from local processes to WAN */
             CLOUD_ACCESS,                   /* @brief For authorising Cloud servers */
             BLOCKED_SITES,                  /* @brief For general firewalling & security */
             PARENTAL_CONTROL,               /* @brief For custom parental control rules */
             QOS_MANAGEMENT,                 /* @brief For traffic prioritisation */
             MAPT,                           /* @brief For NAT46 rules */
             LAN_HIJACK                      /* @brief For troubleshooting/self heal scenarios when LAN client attempts to access the Internet */
         };
 
         using IStringIterator = RPC::IIteratorType<string, RPC::ID_STRINGITERATOR>;
 
         ~IFirewallControl() override = default;
 
         // @brief Configure rules for a given service. This atomically sets all the rules for the given service in one go, overriding the previous configuration.
         // NOTE: This will NOT impact rules for any other services. This will NEVER flush the entire firewall. It will ONLY modify rules for the given service.
         // WARNING: THIS WILL FLUSH THE CURRENT RULESET FOR THE GIVEN SERVICE. Update only if you're certain you won't be inadvertently removing any critical rules!
         //
         // @param service_id: name of the service that these rules apply to
         // @param file: path or URL of the file containing the list of rules to configure for the given service. This new file will override the existing
         //
         // @example ConfigureRules(ServiceType::MAPT, "/tmp/mapt_rules.txt")
         // @example ConfigureRules(ServiceType::BLOCKED_SITES, "https://operator.cloud/blocked_sites.txt")
         virtual void ConfigureRules(const ServiceType& service_id /* @index */, const String& file /* @in */) = 0;
 
         // @brief List all supported services
         virtual uint32_t ListSupportedServices(IStringIterator*& service_ids /* @out */) const = 0;
 
         // @brief List all currently enabled services
         virtual uint32_t ListEnabledServices(IStringIterator*& service_ids /* @out */) const = 0;
 
         // @brief Activate any pre-configured rules for a given service
         // @param service_id: name of the service to be enabled
         virtual void EnableService(const ServiceType& service_id /* @index */) = 0;
 
         // @brief Deactivate all configured rules for a given service. This is non-destructive - it disables the service without flushing the rules!
         // @param service_id: name of the service to be disabled
         virtual void DisableService(const ServiceType& service_id /* @index */) = 0;
 
         // @brief List rules for a given service
         // @param service_id: name of the service to fetch the rules for
         virtual void ListRules(const ServiceType& service_id /* @index */) = 0;
 
         // @brief Delete all rules for a given service. WARNING: This is DESTRUCTIVE - be careful before you call this function.
         // NOTE: This will NOT impact rules for any other services. This will NEVER flush the entire firewall. It will ONLY flush rules for the given service.
         // @param service_id: name of the service to be flushed
         virtual void FlushRules(const ServiceType& service_id /* @index */) = 0;
  
     };   
 }
 }