//
//  XMPPDispatcher.h
//  Jabber
//
//  Created by David Chisnall on Sun May 23 2004.
//  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <EtoileXML/ETXMLNode.h>
#import "XMPPMessage.h"
#import "XMPPInfoQueryStanza.h"
#import "XMPPPresence.h"

/**
 * Formal protocol for handling messages.
 */
@protocol MessageHandler <NSObject>
/**
 * Handle the given message object.
 */
- (void) handleMessage:(XMPPMessage*)aMessage;
@end
/**
 * Formal protocol for handling presence stanzas.
 */
@protocol PresenceHandler <NSObject>
/**
 * Handle the given presence object.
 */
- (void) handlePresence:(XMPPPresence*)aPresence;
@end
/**
 * Formal protocol for handling info-query packets.
 */
@protocol XMPPInfoQueryStanzaHandler <NSObject>
/**
 * Handle the given info-query object.
 */
- (void) handleInfoQuery:(XMPPInfoQueryStanza*)anIq;
@end

/**
 * The XMPPDispatcher class is used for routing of received stanzas.  Once each 
 * stanza has been passed, it will be encapsulated in an object.  There are 
 * three kinds of stanza specified by XMPP:
 *
 * - Messages are one-to-one and have best-effort delivery semantics.  They
 * will typically be enqueued if they can not be delivered immediately
 *
 * - Presences are one-to-many and are unreliable.
 *
 * - Info-query (iq) stanzas are one-to-one, will not be enqueued, but should
 * be acknowledged with either an error or a result reply.
 */
@interface XMPPDispatcher : NSObject {
	NSMutableDictionary * iqHandlers;
	NSMutableDictionary * iqNamespaceHandlers;
	NSMutableDictionary * messageHandlers;
	NSMutableDictionary * presenceHandlers;
	id <XMPPInfoQueryStanzaHandler> defaultIqHandler;
	id <MessageHandler> defaultMessageHandler;
	id <PresenceHandler> defaultPresenceHandler;
}
/**
 * Create a new dispatcher with default handlers.  In the current implementation
 * all of these will be set to DefaultHandler objects.
 */
+ (id) dispatcherWithDefaultInfoQueryHandler:(id <XMPPInfoQueryStanzaHandler>)iq 
					   messageHandler:(id <MessageHandler>)message 
					  presenceHandler:(id <PresenceHandler>)presence;
/** 
 * Initialise a new dispatcher with default handlers 
 */
- (id) initWithDefaultInfoQueryHandler:(id <XMPPInfoQueryStanzaHandler>)iq 
				 messageHandler:(id <MessageHandler>)message 
				presenceHandler:(id <PresenceHandler>)presence;
/**
 * Add a handler for info-query packets with the type 'result' or 'error'.  Each
 * outgoing packet should have a unique ID (which can be generated by the 
 * connection object if required).  This ID will be re-used for the reply.  This
 * method should be used to set up handlers for replies to queries sent to
 * remote clients.
 */
- (id) addInfoQueryResultHandler:(id <XMPPInfoQueryStanzaHandler>)handler forID:(NSString*)iqID;
/**
 * Add a handler for info-query packets with the type 'set' or 'get' in the 
 * specified namespace.
 */
- (id) addInfoQueryHandler:(id <XMPPInfoQueryStanzaHandler>)handler forNamespace:(NSString*)aNamespace;
/**
 * Add a message handler for a specified JID.  All messages sent by this JID 
 * will be routed to the specified handler.  If a JID with no resource is 
 * specified, then it will receive messages sent by all resources for the given
 * JID.
 */
- (id) addMessageHandler:(id <MessageHandler>)handler ForJID:(NSString*)jid;
/**
 * Add a presence handler with the same delivery semantics as a message handler.
 */
- (id) addPresenceHandler:(id <PresenceHandler>)handler ForJID:(NSString*)jid;
/**
 * Dispatch the specified message object to anyone who has elected to receive 
 * it.
 */
- (void) dispatchMessage:(XMPPMessage*)aMessage;
/**
 * Dispatch the specified presence object to anyone who has elected to receive 
 * it.
 */
- (void) dispatchPresence:(XMPPPresence*)aPresence;
/**
 * Dispatch the specified info-query object to anyone who has elected to 
 * receive it.
 */
- (void) dispatchInfoQuery:(XMPPInfoQueryStanza*)anIq;

@end
