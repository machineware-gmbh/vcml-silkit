/******************************************************************************
 *                                                                            *
 * Copyright (C) 2024 MachineWare GmbH                                        *
 * All Rights Reserved                                                        *
 *                                                                            *
 * This is work is licensed under the terms described in the LICENSE file     *
 * found in the root directory of this source tree.                           *
 *                                                                            *
 ******************************************************************************/

#ifndef VCML_SILKIT_SILKIT_H
#define VCML_SILKIT_SILKIT_H

namespace SilKit {

class IParticipant;

namespace Services {
namespace Ethernet {

class IEthernetController;

} // namespace Ethernet

namespace Orchestration {

class ILifecycleService;
class ITimeSyncService;

} // namespace Orchestration
} // namespace Services
} // namespace SilKit

#endif
