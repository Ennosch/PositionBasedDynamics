#include "include/dynamics/softBody.h"

softBody::softBody()
{

}

softBody::softBody(ModelPtr _model)
{
    m_model = std::make_shared<Model>();
    m_model->clone(_model);
}
