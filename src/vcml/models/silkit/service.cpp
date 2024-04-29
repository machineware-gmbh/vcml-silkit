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
