## 1.0.0 (2022-05-22)


### Features

* :art: added status output for wifi and chip data ([d69d1e6](https://github.com/Mario-F/esp-fancontroller/commit/d69d1e6ebad522e076c9f73d800b4b001d841ded))
* :art: moved fan functions to own controller class ([935127f](https://github.com/Mario-F/esp-fancontroller/commit/935127f24fae7acba9bc1cef40f75b0bdaf1629e))
* :rocket: progress on configuration manager ([7881e2b](https://github.com/Mario-F/esp-fancontroller/commit/7881e2be0a1dddb4992f8db848023550b6f6fbee))
* :tada: begin work on config manager ([cb3c988](https://github.com/Mario-F/esp-fancontroller/commit/cb3c9884f14cba346a7c8f8f8d19c752608c9d75))
* :white_check_mark: moved json responses to ArduinoJson ([25253c7](https://github.com/Mario-F/esp-fancontroller/commit/25253c721c5412c7e3ed9f4f043d014e6bdd787b))
* added basic metrics (prom) endpoint ([dabcb13](https://github.com/Mario-F/esp-fancontroller/commit/dabcb13324245bd66e729920437ce46721af684a))
* added endpoint to set target sensor ([8ea7231](https://github.com/Mario-F/esp-fancontroller/commit/8ea723110422440d83be419283e85b96e1561a98))
* added function to set target temp ([1a736ce](https://github.com/Mario-F/esp-fancontroller/commit/1a736ce7e426682b6dfc4689cf2a2b665bd8ac39))
* added http hint ([f69facf](https://github.com/Mario-F/esp-fancontroller/commit/f69facfd5607e1c8681c1aa31448a224536f92cd))
* added instance naming function ([1221135](https://github.com/Mario-F/esp-fancontroller/commit/1221135f71680052e9a6b277e079cc185640ef04))
* added littlefs config file ([a7b2257](https://github.com/Mario-F/esp-fancontroller/commit/a7b2257ad6ee99b8aeda5d129869e67d94719fc1))
* added MetricsManager ([a68700c](https://github.com/Mario-F/esp-fancontroller/commit/a68700c6439b438ca3a5afce557c4270e2bf40d3))
* added save and load for sensors config ([fbf5647](https://github.com/Mario-F/esp-fancontroller/commit/fbf5647b08fc36d793b7092a66f029a73809f410))
* added semantic-release ([df99ef0](https://github.com/Mario-F/esp-fancontroller/commit/df99ef027b271fb344b72f5a1609033454dce289))
* added state of development ([9d5edda](https://github.com/Mario-F/esp-fancontroller/commit/9d5edda11bf2e14335f78416197fb627b56b3011))
* **fan:** added invertable pwm ([f1688ce](https://github.com/Mario-F/esp-fancontroller/commit/f1688ce9424de60b3bc60a4ed3fafacaa6be249c))
* implement config save and default fanspeed ([dad6101](https://github.com/Mario-F/esp-fancontroller/commit/dad6101de81df73105e113d472e79db4e4fe74ba))
* implement initial fan temp regulation loop and min speed ([3d8098d](https://github.com/Mario-F/esp-fancontroller/commit/3d8098d8f54da745e140aee63d695d9587b85399))
* implement persistent sensor config names ([553c988](https://github.com/Mario-F/esp-fancontroller/commit/553c988b554092a35305f27facc4c0887cbb739a))
* **kicad:** added external esp8266 schema lib ([92a5456](https://github.com/Mario-F/esp-fancontroller/commit/92a54567cf7fb0318d002e41d7b1d69032f7264d))
* **kicad:** added fan output ([728274c](https://github.com/Mario-F/esp-fancontroller/commit/728274c60731fd144802d97b685053abbf6846fc))
* **kicad:** added folder with gitignore ([f87e762](https://github.com/Mario-F/esp-fancontroller/commit/f87e7622ff89ff17e8840cf7b009b8f1a9ee78b3))
* **kicad:** added initial schema ([83ac9ba](https://github.com/Mario-F/esp-fancontroller/commit/83ac9baa391b04ac0164315ea2143bda18726259))
* **kicad:** added inverted pwm with npn transistor ([26a1dd7](https://github.com/Mario-F/esp-fancontroller/commit/26a1dd7429f6dc3bcad4b86b78ffa68bf717dd3f))
* **kicad:** added main power connector ([cacbbb5](https://github.com/Mario-F/esp-fancontroller/commit/cacbbb5a536033471374252cbc72831ff4be9d0d))
* **kicad:** added power supply ([2212b71](https://github.com/Mario-F/esp-fancontroller/commit/2212b7137a7b87f6feb7ddc53b19c53b456bd0fa))
* **kicad:** added schmatic pdf ([f162809](https://github.com/Mario-F/esp-fancontroller/commit/f1628093d51be28d57f0f11621bcca81c8a14f14))
* **kicad:** added temp sensor circut ([7f1dbff](https://github.com/Mario-F/esp-fancontroller/commit/7f1dbffcfd1cf33a3d86ea3ccafdd922216403ff))
* **kicad:** tidy up shematic ([72aebfe](https://github.com/Mario-F/esp-fancontroller/commit/72aebfe30c0013fe253f4c1a0b26db9ac45b828e))
* **metrics:** added esp stats ([e9ab47f](https://github.com/Mario-F/esp-fancontroller/commit/e9ab47f6b84f04db90c6bfbd70e5b267e7e282d8))
* **metrics:** added local wlan ip ([800d655](https://github.com/Mario-F/esp-fancontroller/commit/800d655ff1cc420984c51e188afeb5541f20c143))
* **metrics:** added target temp for selected sensor ([236e972](https://github.com/Mario-F/esp-fancontroller/commit/236e972583302a9cf2b1a0c2e3bbf6722512e1d9))
* more intelligent fan temp step up process ([fc04ccf](https://github.com/Mario-F/esp-fancontroller/commit/fc04ccf337ee6d927e31baa42326d6ce63cb6d32))
* more prometheus metrics functions ([61698de](https://github.com/Mario-F/esp-fancontroller/commit/61698de40315ce0e4a8f76d6c17465308baedcc8))
* reduce sample rate for temp sensors ([611b1be](https://github.com/Mario-F/esp-fancontroller/commit/611b1be993f6deafc19d2a1083b04f86eb20d853))
* some cleanups and improvements ([0b6a78a](https://github.com/Mario-F/esp-fancontroller/commit/0b6a78a0fc1936c3c375fe1ce348cee30e22c3b4))
* update analogWrite to new range 0 to 255 ([175f3df](https://github.com/Mario-F/esp-fancontroller/commit/175f3dfab191ccee57bab3a42f32da30c6a343cb))
* use camel case for config json rep ([58247b5](https://github.com/Mario-F/esp-fancontroller/commit/58247b5ff6fd0d7be5a7e8624a9683cd8872fb39))
* use compiler flags for debugmode ([0f9bdea](https://github.com/Mario-F/esp-fancontroller/commit/0f9bdeae562d5de2f0a3947f590119d69439b4ef))
* verbose mode also on temp loop ([d905647](https://github.com/Mario-F/esp-fancontroller/commit/d905647e5b7df2d6193a5422f1ee1f4e414c0964))


### Bug Fixes

* :bug: onewire should be scoped at classlevel ([6e56aa8](https://github.com/Mario-F/esp-fancontroller/commit/6e56aa8d7cba68047ead7d030c22d621470209cf))
* better error handling for fan temp loop ([21f04ca](https://github.com/Mario-F/esp-fancontroller/commit/21f04ca67cc4be91ace7e8c98e554b241bc966bd))
* dont output fanspeed serial on get request ([6bda713](https://github.com/Mario-F/esp-fancontroller/commit/6bda713d13f1f55256c35361e4e17e642588f5df))
* **fan:** set pwm out pinMode before first write ([0f1e6aa](https://github.com/Mario-F/esp-fancontroller/commit/0f1e6aac7f881808fc22515c3c2667ff2f65d3a7))
* fixed log prefix red naming ([d08df0b](https://github.com/Mario-F/esp-fancontroller/commit/d08df0bfed3fa54dd35a2d925051af2386fc62dd))
* **kicad:** added pullup on pwm control because of be quiet ([981de71](https://github.com/Mario-F/esp-fancontroller/commit/981de71f2e0fed5299fa35c4e5fc0835dac6b3b7))
* **kicad:** name unnamed devices ([5a42e00](https://github.com/Mario-F/esp-fancontroller/commit/5a42e00d646533ecb777d8db2e6bd64940316bd0))
* **kicad:** named 1wire 2wire ([e9aa468](https://github.com/Mario-F/esp-fancontroller/commit/e9aa468d45ad9fc5eb63f770fa921a602983499f))
* **metrics:** same metrics must be grouped together ([d09b2af](https://github.com/Mario-F/esp-fancontroller/commit/d09b2af51830947a46fd3a2c87188bcbf86301f2))
* new analogWrite range ([e8b7ae0](https://github.com/Mario-F/esp-fancontroller/commit/e8b7ae0d83d044cb6ae39f36f4ba91f76c160dde))
* removed config experiments ([ca8d4a6](https://github.com/Mario-F/esp-fancontroller/commit/ca8d4a61507584ef3b6bec8f546107f102cf05ec))
* removed npm token ([1d278bf](https://github.com/Mario-F/esp-fancontroller/commit/1d278bfd86c962d916e0bf549578e191fc670725))
* removed red information ([83fc22a](https://github.com/Mario-F/esp-fancontroller/commit/83fc22a5f7d8f52d4305196af53a6f04772fbc9e))
* set fan 100 on startup ([58582a7](https://github.com/Mario-F/esp-fancontroller/commit/58582a7da4e4741b02118bc7189db2b4cb57d073))


### Performance Improvements

* added stepdown skip logic ([573da23](https://github.com/Mario-F/esp-fancontroller/commit/573da23bdc4ef04f71551cb3e945672d82c49853))
* added upskip logic to temp loop ([4167d8c](https://github.com/Mario-F/esp-fancontroller/commit/4167d8c7f0df4405432836d8e732e8a1357903fd))
* decrease temploop executions time ([bc13904](https://github.com/Mario-F/esp-fancontroller/commit/bc13904521261b8f1816e53be4dcb81c1834cb8a))
* **ftloop:** tune loop parameters ([cc848d7](https://github.com/Mario-F/esp-fancontroller/commit/cc848d74841e2cc2d2832cb6d07dde3b384d7da2))
* half the fan/temp control loop reaction time ([f379fb4](https://github.com/Mario-F/esp-fancontroller/commit/f379fb4bdade02c73c010f61dc7ad72154fd4579))
* increase temploop skip steps ([b28e2e2](https://github.com/Mario-F/esp-fancontroller/commit/b28e2e26d1c6aba5d49191fd84d1656459b995bf))
* increase temploop stepup ([398f4f9](https://github.com/Mario-F/esp-fancontroller/commit/398f4f99a2e885a4dad41391c710591f2238b3ec))
* **kicad:** 10k for pwm drive is even better ([c96418f](https://github.com/Mario-F/esp-fancontroller/commit/c96418fe368e2168c504c7a671e7fd5139427dff))
* **kicad:** use 5k resistor for pwm drive ([e434b6e](https://github.com/Mario-F/esp-fancontroller/commit/e434b6e438437f11f2603b056cf54fcf6df095ef))
* **metrics:** increase coutns a bit more ([a86a08c](https://github.com/Mario-F/esp-fancontroller/commit/a86a08c53f9c541a6502b1b4efb281b71589ffff))
* **metrics:** increase max metrics and local labels ([3f338b4](https://github.com/Mario-F/esp-fancontroller/commit/3f338b47956a25fc4fd0f9a492159fc6ab32e953))
* **metrics:** refined metric counts a bit more ([bfa97f7](https://github.com/Mario-F/esp-fancontroller/commit/bfa97f77fad216ddb7b7062867010b00565ae08d))
* tuning on fan temp loop ([8af41a0](https://github.com/Mario-F/esp-fancontroller/commit/8af41a02293addf9e3793e6ae17d0f5d5b3bacc3))


### Malformed Commits

* Merge pull request #1 from Mario-F/dev ([b6de396](https://github.com/Mario-F/esp-fancontroller/commit/b6de396a8fa3e6eefd36493818517a7a5db2e971)), closes [#1](https://github.com/Mario-F/esp-fancontroller/issues/1)
* first commit ([b9b03fb](https://github.com/Mario-F/esp-fancontroller/commit/b9b03fb065cffcc9b10f9b39b48116efde4b4a15))


### Miscellaneous

* added http endpoints to docs ([7219022](https://github.com/Mario-F/esp-fancontroller/commit/72190224dd2d3a3f97371eb4605cf23a8dfabfb9))
* added inline todo ([aa65e2c](https://github.com/Mario-F/esp-fancontroller/commit/aa65e2c744604454cef01be23bca90033cf55259))
* added project state to readme ([2589699](https://github.com/Mario-F/esp-fancontroller/commit/25896990bccbe5ba9715cdb9f8fb9eb24c5dc5f3))
* added release file ([e5f0ce8](https://github.com/Mario-F/esp-fancontroller/commit/e5f0ce801c74eb71eece4cae0100ce341395aaa4))
* added release workflow ([5b5946f](https://github.com/Mario-F/esp-fancontroller/commit/5b5946fe9a20f421423898cdd153957a676c4b81))
* added todo ([422fd46](https://github.com/Mario-F/esp-fancontroller/commit/422fd461d4f93397416edbdf6056ae8394693b0e))
* encapsulate link ([a562409](https://github.com/Mario-F/esp-fancontroller/commit/a562409d1e9a2e01ee5ea788884a28e16fdaec54))
* fixed typo ([b079676](https://github.com/Mario-F/esp-fancontroller/commit/b07967676a4c32589cbe83d6484051b36c4836c7))
* fixed typo in http test ([f38e26c](https://github.com/Mario-F/esp-fancontroller/commit/f38e26cb24b00a106287dbc378ca5f8f9ce4458f))
* improve readme ([23e2910](https://github.com/Mario-F/esp-fancontroller/commit/23e2910bf19375d5f55b045ba9354fb75e2ec780))
* **kicad:** added schematic link to README ([9d0f55b](https://github.com/Mario-F/esp-fancontroller/commit/9d0f55bf6c6592fdc2a16da5a235d2c676b18b29))
* make clear temp sensor addr/name to uid ([4f38324](https://github.com/Mario-F/esp-fancontroller/commit/4f383243a86daf0a052ce7ca790864abf845e347))
* remove npm from release file ([1c8754c](https://github.com/Mario-F/esp-fancontroller/commit/1c8754c9388a3e16d8444603b448ddab5af91e29))
* start test with http request files ([1174c11](https://github.com/Mario-F/esp-fancontroller/commit/1174c112fc4abf43c0d3594499cad9f7885e5b8c))
