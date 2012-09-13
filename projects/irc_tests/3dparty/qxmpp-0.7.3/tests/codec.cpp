/*
 * Copyright (C) 2008-2012 The QXmpp developers
 *
 * Authors:
 *  Jeremy Lainé
 *
 * Source:
 *  http://code.google.com/p/qxmpp
 *
 * This file is a part of QXmpp library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */

#include "QXmppCodec_p.h"

#include "codec.h"

void TestCodec::testTheoraDecoder()
{
#ifdef QXMPP_USE_THEORA
    QMap<QString, QString> params;
    params.insert("delivery-method", "inline");
    params.insert("configuration", "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");

    QXmppTheoraDecoder decoder;
    QCOMPARE(decoder.setParameters(params), true);

    QXmppVideoFormat format = decoder.format();
    QCOMPARE(format.frameSize(), QSize(320, 240));
    QCOMPARE(format.pixelFormat(), QXmppVideoFrame::Format_YUV420P);
#endif
}

void TestCodec::testTheoraEncoder()
{
#ifdef QXMPP_USE_THEORA
    QXmppVideoFormat format;
    format.setFrameSize(QSize(320, 240));
    format.setPixelFormat(QXmppVideoFrame::Format_YUV420P);

    QXmppTheoraEncoder encoder;
    encoder.setFormat(format);

    QMap<QString, QString> params = encoder.parameters();
    QCOMPARE(params.value("delivery-method"), QLatin1String("inline"));
    QCOMPARE(params.value("configuration"), QLatin1String("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"));
#endif
}
