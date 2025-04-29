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

namespace WPEFramework {
namespace Exchange {

    // @json
    struct EXTERNAL IWifiAPControl : virtual public Core::IUnknown {
        enum { ID = ID_WIFIAPCONTROL };

	enum RadioFreqency: uint8_t {
	    RADIO_2_4 = 0,
	    RADIO_5,
	    RADIO_6
	};

        enum Key : uint8_t {
            PSK        = 0x01  /* @text: PSK */,
            EAP        = 0x02  /* @text: EAP */,
            CCMP       = 0x04  /* @text: CCMP */,
            TKIP       = 0x08  /* @text: TKIP */,
            PREAUTH    = 0x10  /* @text: PREAUTH */,
            PBC        = 0x20  /* @text: PBC */,
            PIN        = 0x40  /* @text: PIN */,
            PSK_HASHED = 0x80  /* @text: PSK_HASHED */,
            NONE       = 0x00
        };

        enum Security : uint8_t {
            OPEN       = 0x01,
            WEP        = 0x02  /* @text: WEP */,
            WPA        = 0x04  /* @text: WPA */,
            WPA2       = 0x08  /* @text: WPA2 */,
            WPS        = 0x10  /* @text: WPS */,
            ENTERPRISE = 0x20  /* @text: Enterprise */,
            WPA_WPA2   = 0x40  /* @text: WPA_WPA2 */,
	    WPA3       = 0x80  /* @text: WPA3 */,
            UNKNOWN    = 0x00
        };

        struct VapConfig {
	    RadioFreqency radioFrequency;  //Radio Frequency of the vap
	    string vapName;                //Vap Name/Interface Name
            string ssid;                   //Ssid name for the vap
	    Security securitymethod;       //Security method for the vap
	    Key keyBitmap;                 //Security key for the vap
	    string passphrase;             //Security password for the vap
	    bool enable;                   //Enable/Disable each vap
        };

        // @event
        struct EXTERNAL INotification : virtual public Core::IUnknown {
            enum { ID = ID_WIFIAPCONTROL_NOTIFICATION };
            ~INotification() override = default;
           
            // @brief Notifies that vap status has changed
	    virtual uint32_t VapStatusChanged(const string vapName, const bool status) = 0;

        };
         ~IWifiAPControl() override = default;

        virtual uint32_t Register(IWifiAPControl::INotification* sink) = 0;
        virtual uint32_t Unregister(IWifiAPControl::INotification* sink) = 0;

        // @property
	// @brief Create a vap on the radio with the vap configurations provided
	virtual uint32_t CreateVap(const VapConfig& vapConfig) = 0;
	
        // @property
	// @brief Delete a vap on the radio with the vap configurations provided
	virtual uint32_t DeleteVap(const string vapName /* @index */) = 0;
	
        // @property
        // NOTE: Setting the config by indicating the vapindex.
        // @brief Provide config details for requested vapindex.
	virtual uint32_t Config(const string vapName /* @index */, VapConfig& vapConfig /* @out */) const = 0;
	virtual uint32_t Config(const string vapName /* @index */, const VapConfig& vapConfig) = 0;

        // @brief Enable the vap for provided vapName
        virtual uint32_t Enable(const string vapName /* @index */) = 0;

        // @brief Disable the vap for provided vapName
        virtual uint32_t Disable(const string vapName /* @index */) = 0;

    };

} // namespace Exchange
} // namespace WPEFramework
