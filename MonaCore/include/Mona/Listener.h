/*
Copyright 2014 Mona
mathieu.poux[a]gmail.com
jammetthomas[a]gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License received along this program for more
details (or else see http://www.gnu.org/licenses/).

This file is a part of Mona.
*/

#pragma once

#include "Mona/Mona.h"
#include "Mona/Writer.h"
#include "Mona/QualityOfService.h"
#include "Mona/Client.h"
#include "Mona/MapParameters.h"
#include "Mona/DataReader.h"

namespace Mona {

class Publication;
class Listener : public MapParameters, virtual Object {
public:

	Listener(Publication& publication,Client& client,Writer& writer);
	virtual ~Listener();

	void seek(UInt32 time);

	void startPublishing();
	void stopPublishing();

	void pushAudio(UInt32 time,PacketReader& packet); 
	void pushVideo(UInt32 time,PacketReader& packet);
	void pushData(DataReader& packet);
	void pushProperties(DataReader& packet);

	void flush();

	bool receiveAudio;
	bool receiveVideo;

	const Publication&	publication;
	Client&				client;

	const QualityOfService&	videoQOS() const { return _pVideoWriter ? _pVideoWriter->qos() : QualityOfService::Null; }
	const QualityOfService&	audioQOS() const { return _pAudioWriter ? _pAudioWriter->qos() : QualityOfService::Null; }
	const QualityOfService&	dataQOS() const { return _writer.qos(); }

private:
	enum {
		DATA_NONE=0,
		DATA_INITIALIZED=1,
		DATA_WASRELIABLE=2,
		DATA_RELIABLE=4
	};

	void    writeData(DataReader& reader,Writer::DataType type);

	bool	initWriters();
	void	closeWriters();

	void	firstMedia(UInt32 time);

	PacketReader& publicationNamePacket() { _publicationNamePacket.reset(); return _publicationNamePacket; }

	bool					_firstMedia;
	UInt32 					_startTime;
	UInt32					_lastTime;
	bool					_firstTime;
	UInt32					_seekTime;
	bool					_codecInfosSent;
	
	Writer&					_writer;
	Writer*					_pAudioWriter;
	Writer*					_pVideoWriter;
	UInt8					_dataInfos;
	PacketReader			_publicationNamePacket;
};


} // namespace Mona
