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

#ifndef VCML_SILKIT_SERVICE_H
#define VCML_SILKIT_SERVICE_H

#include "vcml/core/types.h"
#include "vcml/logging/logger.h"

#include "vcml/models/silkit/participant.h"

namespace vcml {
namespace silkit {

class service : public module
{
protected:
    string m_type;

public:
    logger& log;

    const char* type() const { return m_type.c_str(); }

    service(participant& part, const sc_module_name& nm);
    virtual ~service();

    service() = delete;
    service(const service&) = delete;
    service(service&&) = default;
};

} // namespace silkit
} // namespace vcml

#endif
