// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "CurveProcessModel.hpp"

namespace Curve
{
CurveProcessModel::~CurveProcessModel()
{
  emit identified_object_destroying(this);
}
}
