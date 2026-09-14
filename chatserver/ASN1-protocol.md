UPSChatProtocol DEFINITIONS ::= BEGIN

	MessageType ::= IA5String(
		"nick" |
		"chat" |
		"conn" |
		"disc" |
		"exit"
	)
	
	NickType ::= IA5String(SIZE(4..32))
	
	MessageHeader { MessageType:Type } ::= SEQUENCE {
		magic       IA5String ("KIVUPS"),
		command     Type,
		totalLength INTEGER(0..9999)
	}

	NickMessage ::= SEQUENCE {
		header      MessageHeader{{"nick"}},
		newNick     NickType
	}
	
	NickMessageSrv ::= SEQUENCE {
		header      MessageHeader{{"nick"}},
		newNick     NickType,
		oldNick     NickType
	}
	
	ChatMessage ::= SEQUENCE {
		header      MessageHeader{{"chat"}},
		message     IA5String (SIZE(1..999))
	}
	
	ChatMessageSrv ::= SEQUENCE {
		header      MessageHeader{{"chat"}},
		nick		NickType,
		message     IA5String (SIZE(1..999))
	}
	
	ConnDisconnMessage ::= SEQUENCE {
		header      MessageHeader{{"conn"}},
		nick        NickType
	}

END