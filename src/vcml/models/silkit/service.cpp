/******************************************************************************
*                                                                            *
* Copyright 2024 MachineWare GmbH                                            *
* All Rights Reserved                                                        *
*                                                                            *
* This is unpublished proprietary work and may not be used or disclosed to   *
* third parties, copied or duplicated in any form, in whole or in part,      *
* without prior written permission of the authors.                           *
*                                                                            *
******************************************************************************/

#include "vcml/models/silkit/service.h"

namespace vcml {
namespace silkit {

service::service(participant& part, const sc_module_name& nm):
    module(nm), m_type("unknown"), log(part.log) {
}

service::~service() {
}

} // namespace silkit
} // namespace vcml
