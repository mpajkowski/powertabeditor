/*
 * Copyright (C) 2014 Cameron White
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ACTIONS_ADDINSTRUMENT_H
#define ACTIONS_ADDINSTRUMENT_H

#include <QUndoCommand>
#include <score/instrument.h>

class Score;

class AddInstrument : public QUndoCommand
{
public:
    AddInstrument(Score &score, const Instrument &instrument);

    virtual void redo() override;
    virtual void undo() override;

private:
    Score &myScore;
    const Instrument myInstrument;
    const int myIndex;
};

#endif
