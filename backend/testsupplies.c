/*
 * SNMP supplies test program for CUPS.
 *
 * Copyright © 2008-2011 by Apple Inc.
 *
 * Licensed under Apache License v2.0.  See the file "LICENSE" for more
 * information.
 */

/*
 * Include necessary headers.
 */

#include "backend-private.h"

#define CUPS_MAX_SUPPLIES	32
/*
 * 'main()' - Show the supplies state of a printer.
 */

int					/* O - Exit status */
main(int  argc,				/* I - Number of command-line args */
     char *argv[])			/* I - Command-line arguments */
{
  http_addrlist_t	*host;		/* Host addresses */
  int			snmp_fd;	/* SNMP socket */
  int			page_count,	/* Current page count */
			printer_state;	/* Current printer state */

  if (argc != 2)
  {
    puts("Usage: testsupplies ip-or-hostname");
    return (1);
  }

  if ((host = httpAddrGetList(argv[1], AF_UNSPEC, "9100")) == NULL)
  {
    perror(argv[1]);
    return (1);
  }

  if ((snmp_fd = _cupsSNMPOpen(host->addr.addr.sa_family)) < 0)
  {
    perror(argv[1]);
    return (1);
  }

//  for (;;)
//  {
	  char *new_state_str = NULL;
	  char *new_supply_str = NULL;
	  char	marker_levels[CUPS_MAX_SUPPLIES * 4] = {0};
    fputs("backendSNMPSupplies: ", stdout);

    if (backendSNMPGetState(snmp_fd, &(host->addr),
                            &page_count,
                            &printer_state,
                            &new_supply_str,
                            &new_state_str,
                            marker_levels)){
        puts("FAIL");
        return (1);
    }

    printf("result=%s,"
            "page_count=%d, "
            "printer_state=%d, "
            "new_supply_str=%s, "
            "new_state_str=%s, "
            "marker_levels=%s)\n",
            page_count < 0 || printer_state < CUPS_TC_other || printer_state > CUPS_TC_warmup ? "fail" : "ok",
            page_count,
            printer_state,
            new_supply_str,
            new_state_str,
            marker_levels);

//    sleep(5);
//  }
    return 0;
}
