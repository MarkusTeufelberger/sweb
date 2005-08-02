//----------------------------------------------------------------------
//  $Id: events.c,v 1.1 2005/08/01 08:22:38 nightcreature Exp $
//----------------------------------------------------------------------
//
//  $Log: events.c,v $
//
//----------------------------------------------------------------------

/* -*-  Mode:C; c-basic-offset:4; tab-width:4 -*-
 ****************************************************************************
 * (C) 2003 - Rolf Neugebauer - Intel Research Cambridge
 ****************************************************************************
 *
 *        File: events.c
 *      Author: Rolf Neugebauer (neugebar@dcs.gla.ac.uk)
 *     Changes: 
 *              
 *        Date: Jul 2003
 * 
 * Environment: Xen Minimal OS
 * Description: Deal with events
 *
 ****************************************************************************
 * $Id: events.c,v 1.1 2005/08/01 08:22:38 nightcreature Exp $
 ****************************************************************************
 */

#include "os.h"
#include "hypervisor.h"
#include "events.h"
#include "stdarg.h"
#include "lib.h"

static ev_action_t ev_actions[NR_EVS];
void default_handler(int ev, struct pt_regs *regs);


/*
 * demux events to different handlers
 */
unsigned int do_event(int ev, struct pt_regs *regs)
{
    ev_action_t  *action;
    char buffer[256];

    if (ev >= NR_EVS) {
        printf("Large event number %d\n", ev);
    return 0;
    }

    action = &ev_actions[ev];
    /*action->count++;*/
    /* ack_hypervisor_event(ev); */

    if (!action->handler)
    goto out;

    
    if (action->status & EVS_DISABLED)
    goto out;

    
    /* call the handler */
    action->handler(ev, regs);
    
 out:
    return 1;

}

/*
 * add a handler
 */
unsigned int add_ev_action( int ev, void (*handler)(int, struct pt_regs *) )
{
    if (ev_actions[ev].handler) {
        printf ("event[%d] already handled by %p", ev, ev_actions[ev].handler);
        return 0;
    }

    ev_actions[ev].handler = handler;
    return 1;
}

unsigned int enable_ev_action( int ev )
{
    if (!ev_actions[ev].handler) {
        printf ("enable event[%d], no handler installed", ev);
        return 0;
    }
    ev_actions[ev].status &= ~EVS_DISABLED;
    return 1;
}

unsigned int disable_ev_action( int ev )
{
    ev_actions[ev].status |= EVS_DISABLED;
    return 1;
}


void default_handler(int ev, struct pt_regs *regs) {

}

/*
 * initially all events are without a handler and disabled
 */
void init_events(void)
{
    int i;

       
    /* inintialise event handler */
    for ( i = 0; i < NR_EVS; i++ )
    {
        ev_actions[i].status  = EVS_DISABLED;
        ev_actions[i].handler = 0;
    }
    
    
}

