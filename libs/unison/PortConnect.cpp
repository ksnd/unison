/*
 * PortConnect.cpp
 *
 * Copyright (c) 2010 Paul Giblock <pgib/at/users.sourceforge.net>
 *
 * This file is part of Unison - http://unison.sourceforge.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */


#include "unison/PortConnect.h"
#include "unison/ProcessingContext.h"

namespace Unison {
  namespace Internal {

PortConnect::PortConnect (Port *port1, Port *port2) :
  Command(),
  m_producer(NULL),
  m_consumer(NULL),
  m_patch(NULL)
{
  if (port1->direction() == OUTPUT) {
    if (port2->direction() == OUTPUT) {
      qFatal("Cannot connect two output ports");
    }
    m_producer = port1;
    m_consumer = port2;
  }
  else {
    if (port2->direction() == INPUT) {
      qFatal("Cannot connect two input ports");
    }
    m_consumer = port1;
    m_producer = port2;
  }

  m_blocking = false;
  m_compiled = new QList<Patch::CompiledProcessor>();
  m_state = Command::Created;
}


void PortConnect::preExecute ()
{
  // TODO: Check for existing connection and cycles!!!
  m_patch = m_producer->parentPatch();
  // Handle the case where port1 is a backend port, and has no parent patch
  if (!m_patch) {
    m_patch = m_consumer->parentPatch();
  }
  Q_ASSERT(m_patch);
  // TODO Bring back this assertion once BackendPorts have a parent patch
  //Q_ASSERT(m_patch == m_port2->parentPatch());
  Q_ASSERT(!m_producer->isConnected(m_consumer));
  Q_ASSERT(!m_consumer->isConnected(m_producer));

  *m_producer->_connectedPorts() += m_consumer;
  *m_consumer->_connectedPorts() += m_producer;
  
  m_patch->compile(*m_compiled);
  
  Command::preExecute();
}


void PortConnect::execute (ProcessingContext &context)
{
  // Connect Consumer first, to clear out any silence buffer
  m_consumer->connectToBuffer();
  m_producer->connectToBuffer();
  // FIXME: Leaking m_patch->compiledProcessors();
  m_patch->setCompiledProcessors(m_compiled);
  Command::execute(context);
}


void PortConnect::postExecute ()
{
  Command::postExecute();
}

  } // Internal
} // Unison

// vim: ts=8 sw=2 sts=2 et sta noai