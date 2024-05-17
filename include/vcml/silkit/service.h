/******************************************************************************
 *                                                                            *
 * Copyright (C) 2024 MachineWare GmbH                                        *
 * All Rights Reserved                                                        *
 *                                                                            *
 * This is work is licensed under the terms described in the LICENSE file     *
 * found in the root directory of this source tree.                           *
 *                                                                            *
 ******************************************************************************/

#ifndef VCML_SILKIT_SERVICE_H
#define VCML_SILKIT_SERVICE_H

#include "vcml/core/types.h"
#include "vcml/logging/logger.h"

#include "participant.h"

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
};

} // namespace silkit
} // namespace vcml

#endif
