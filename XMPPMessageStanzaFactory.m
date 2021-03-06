//
//  MessageStanzaFactory.m
//  Jabber
//
//  Created by David Chisnall on 24/05/2006.
//  Copyright 2006 __MyCompanyName__. All rights reserved.
//

#import "XMPPMessageStanzaFactory.h"
#import <EtoileXML/ETXMLString.h>
#import <EtoileXML/ETXMLXHTML-IMParser.h>
#import "XMPPError.h"
#ifdef WITH_XMPP_OBJECTSTORE
#import "XMPPObjectStore.h"
#endif
#import "XMPPMessage.h"

static XMPPMessageStanzaFactory * sharedInstance;

@implementation XMPPMessageStanzaFactory
+ (void) initialize
{
	sharedInstance = [[XMPPMessageStanzaFactory alloc] init];
	//Insert default handlers here:
	[sharedInstance addHandler:[ETXMLString class] forTag:@"body"];
	[sharedInstance addHandler:[ETXMLString class] forTag:@"subject"];
	[sharedInstance addHandler:[XMPPTimestamp class] withValue:@"timestamp" forTag:@"x" inNamespace:@"jabber:x:delay"];
	[sharedInstance addHandler:[XMPPError class] forTag:@"error"];
#ifndef WITHOUT_XHTML_IM
	[sharedInstance addHandler:[ETXMLXHTML_IMParser class] forTag:@"html"];
#endif
#ifdef WITH_XMPP_OBJECTSTORE
	[sharedInstance addHandler: [XMPPObjectStore class]
	                    forTag: @"coreobject"
	               inNamespace: @"http://www.etoileos.com/CoreObject"];
#endif
}

+ (id) sharedStazaFactory
{
	return sharedInstance;
}
- (id) parser
{
	return [[XMPPMessage alloc] init];
}
@end
