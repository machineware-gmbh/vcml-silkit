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

// NOLINTBEGIN(readability-identifier-naming)
namespace SilKit {

class IParticipant;

namespace Services {
namespace Ethernet {

class IEthernetController;

} // namespace Ethernet

namespace Can {

class ICanController;

} // namespace Can

namespace Orchestration {

class ILifecycleService;
class ITimeSyncService;
enum class ParticipantState : int16_t;

} // namespace Orchestration
} // namespace Services
} // namespace SilKit

// NOLINTEND(readability-identifier-naming)

#endif
