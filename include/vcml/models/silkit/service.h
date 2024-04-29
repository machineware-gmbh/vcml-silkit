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
