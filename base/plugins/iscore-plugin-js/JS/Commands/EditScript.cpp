// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <JS/JSProcessModel.hpp>
#include <JS/JSStateProcess.hpp>
#include <algorithm>

#include "EditScript.hpp"
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/model/path/PathSerialization.hpp>
namespace JS
{
EditScript::EditScript(const ProcessModel& model, const QString& text)
    : m_model{model}, m_new{text}
{
  m_old = model.script();
}

void EditScript::undo(const iscore::DocumentContext& ctx) const
{
  m_model.find(ctx).setScript(m_old);
}

void EditScript::redo(const iscore::DocumentContext& ctx) const
{
  m_model.find(ctx).setScript(m_new);
}

void EditScript::serializeImpl(DataStreamInput& s) const
{
  s << m_model << m_old << m_new;
}

void EditScript::deserializeImpl(DataStreamOutput& s)
{
  s >> m_model >> m_old >> m_new;
}

EditStateScript::EditStateScript(
    const StateProcess& model, const QString& text)
    : m_model{std::move(model)}, m_new{text}
{
  m_old = model.script();
}

void EditStateScript::undo(const iscore::DocumentContext& ctx) const
{
  m_model.find(ctx).setScript(m_old);
}

void EditStateScript::redo(const iscore::DocumentContext& ctx) const
{
  m_model.find(ctx).setScript(m_new);
}

void EditStateScript::serializeImpl(DataStreamInput& s) const
{
  s << m_model << m_old << m_new;
}

void EditStateScript::deserializeImpl(DataStreamOutput& s)
{
  s >> m_model >> m_old >> m_new;
}
}
