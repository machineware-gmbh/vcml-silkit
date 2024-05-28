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
    participant& m_part;

public:
    logger& log;

    const char* type() const { return m_type.c_str(); }

    participant& part() { return m_part; }

    service(const sc_module_name& nm, participant& part, const char* type);
    virtual ~service();

    service() = delete;
    service(const service&) = delete;
    VCML_KIND(silkit::service);

    virtual const char* version() const override;
};

} // namespace silkit
} // namespace vcml

#endif
