/*
 alert tcp $EXTERNAL_NET any -> $HOME_NET 631 (msg:"EXPLOIT Apple CUPS SGI Image Format Decoding imagetops Filter buffer overflow attempt"; flow:to_server,established; content:"|01 DA 01|"; offset:0; depth:3; metadata: policy security-ips drop, policy balanced-ips drop; reference:CVE,2008-3639; classtype:attempted-user; sid:17663;)

 This rule does NOT use the built-in detection function
*/
/*
 * Use at your own risk.
 *
 * Copyright (C) 2005-2008 Sourcefire, Inc.
 * 
 * This file is autogenerated via rules2c, by Brian Caswell <bmc@sourcefire.com>
 */


#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"

//#define DEBUG
#ifdef DEBUG
#define DEBUG_SO(code) code
#else
#define DEBUG_SO(code)
#endif


/* declare detection functions */
int rule17663eval(void *p);

/* declare rule data structures */
/* precompile the stuff that needs pre-compiled */
/* flow:established, to_server; */
static FlowFlags rule17663flow0 = 
{
    FLOW_ESTABLISHED|FLOW_TO_SERVER
};

static RuleOption rule17663option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule17663flow0
    }
};
// content:"|01 DA 01|", depth 3; 
static ContentInfo rule17663content1 = 
{
    (uint8_t *) "|01 DA 01 01 00 03|", /* pattern (now in snort content format) */
    6, /* depth */
    0, /* offset */
    CONTENT_BUF_NORMALIZED, /* flags */ // XXX - need to add CONTENT_FAST_PATTERN support
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule17663option1 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule17663content1
    }
};

/* references for sid 17663 */
/* reference: cve "2008-3639"; */
static RuleReference rule17663ref1 = 
{
    "cve", /* type */
    "2008-3639" /* value */
};

static RuleReference *rule17663refs[] =
{
    &rule17663ref1,
    NULL
};
/* metadata for sid 17663 */
/* metadata:policy balanced-ips drop, policy security-ips drop; */
static RuleMetaData rule17663service1 =
{
    "service http"
};

static RuleMetaData rule17663policy1 = 
{
    "policy balanced-ips drop"
};

static RuleMetaData rule17663policy2 = 
{
    "policy security-ips drop"
};

static RuleMetaData rule17663policy3 = 
{
    "policy max-detect-ips drop"
};

static RuleMetaData *rule17663metadata[] =
{
    &rule17663service1,
    &rule17663policy1,
    &rule17663policy2,
    &rule17663policy3,
    NULL
};
RuleOption *rule17663options[] =
{
    &rule17663option0,
    &rule17663option1,
    NULL
};

Rule rule17663 = {
   
   /* rule header, akin to => tcp any any -> any any               */{
       IPPROTO_TCP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "any", /* SRCPORT   */
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
       "631", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid (HARDCODED!!!) */
       17663, /* sigid */
       5, /* revision */
   
       "attempted-user", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "SERVER-OTHER Apple CUPS SGI image format decoding imagetops filter buffer overflow attempt",     /* message */
       rule17663refs /* ptr to references */
       ,rule17663metadata
   },
   rule17663options, /* ptr to rule options */
   &rule17663eval, /* use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule17663eval(void *p) {
   const uint8_t *cursor_normal = 0, *cursor_normal_local;
   SFSnortPacket *sp = (SFSnortPacket *) p;
   const uint8_t *beg_of_payload, *end_of_payload;
   uint16_t x_size, y_size, z_size;
   uint32_t offset;
   uint8_t selector_length;
   uint32_t i=0;
   uint32_t sum_length_fields;
   uint32_t tablesize; 

   DEBUG_SO(printf("rule17663eval enter\n"));

   if(sp == NULL)
      return RULE_NOMATCH;

   // flow:established, to_server;
   if(checkFlow(p, rule17663options[0]->option_u.flowFlags) <= 0 )
      return RULE_NOMATCH;

   // content:"|01 DA 01 01 00 03|", depth 6;
   if(contentMatch(p, rule17663options[1]->option_u.content, &cursor_normal) <= 0)
      return RULE_NOMATCH;
   
   if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &beg_of_payload, &end_of_payload) != CURSOR_IN_BOUNDS)
      return RULE_NOMATCH;
 
   if(cursor_normal + 509 >= end_of_payload)
      return RULE_NOMATCH;

   // Get x_size, y_size and z_size from the header
   x_size = *cursor_normal++ << 8;
   x_size |= *cursor_normal++;
   DEBUG_SO(printf("X SIZE: %d\n", x_size));

   y_size = *cursor_normal++ << 8;
   y_size |= *cursor_normal++;
   DEBUG_SO(printf("Y SIZE: %d\n", y_size));

   // z size is number of bit planes
   z_size = *cursor_normal++ << 8;
   z_size |= *cursor_normal++;
   DEBUG_SO(printf("Z SIZE: %d\n", z_size));

   tablesize = y_size * z_size;

   DEBUG_SO(printf("tablesize: %d\n", tablesize));

   if(tablesize < y_size)  // int overflow
      return RULE_NOMATCH;

   cursor_normal += 500;

   // Get offset for each record from the Starttab table. Note that the offsets for records are not sequential
   for (i = 0; i < tablesize; i++) {

      DEBUG_SO(printf("entry %d\n", i));

      if(cursor_normal + 4 >= end_of_payload)
         return RULE_NOMATCH;

      offset = *cursor_normal++ << 24;
      offset |= *cursor_normal++ << 16;
      offset |= *cursor_normal++ << 8;
      offset |= *cursor_normal++;
      DEBUG_SO(printf("OFFSET: %d\n",offset));
   
      cursor_normal_local = beg_of_payload + offset;
      
      if((cursor_normal_local >= end_of_payload) || (cursor_normal_local < beg_of_payload))
         continue; // try the next offset in the table 
    
      sum_length_fields = 0;

      while ((cursor_normal_local < end_of_payload) && *cursor_normal_local) {  
         // Read the SelectorLength byte
         selector_length = *cursor_normal_local++;
         
         // Check to see if we have RLE encoded data
         sum_length_fields += selector_length & 0x7F;

         DEBUG_SO(printf("sum_length_fields=%d\n", sum_length_fields));
         
         if(sum_length_fields > x_size)
          return RULE_MATCH;
        
         // if MSB is set, repeat next byte so only increment pointer by 1
         // if MSB not set, length is rawbytes so skip over them 
         if(!(selector_length & 0x80)){
            cursor_normal_local++;
         } else {
            cursor_normal_local += selector_length & 0x7F;
         }
      }
   }
   
   return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule17663,
    NULL
};
*/
