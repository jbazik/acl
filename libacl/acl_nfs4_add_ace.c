/*
 *  NFSv4 ACL Code
 *  Add an ace to the acl
 *
 *  Copyright (c) 2002, 2003 The Regents of the University of Michigan.
 *  All rights reserved.
 *
 *  Marius Aamodt Eriksen <marius@umich.edu>
 *  J. Bruce Fields <bfields@umich.edu>
 *  Nathaniel Gallaher <ngallahe@umich.edu>
 *  Jeff Sedlak <jsedlak@umich.edu>
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the University nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 *  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "libacl_nfs4.h"

int
acl_nfs4_add_ace(struct nfs4_acl *acl, u32 type, u32 flag, u32 access_mask,
		int whotype, char* who)
{
	struct nfs4_ace *ace;
	int result;

	if(acl == NULL)
	{
		errno = EINVAL;
		return -1;
	}

#ifdef NFSV4_NODENY
	if( type == NFS4_ACE_ACCESS_DENIED_ACE_TYPE )
		return 0;
#endif

	if ((ace = malloc(sizeof(*ace))) == NULL)
	{
		errno = ENOMEM;
		return -1;
	}

	ace->type = type;
	ace->flag = flag;

#ifndef NFSV4_NODENY
	if( type == NFS4_ACE_ACCESS_DENIED_ACE_TYPE )
		access_mask = access_mask & ~(NFS4_ACE_MASK_IGNORE);
#endif


	/* Castrate delete_child if we aren't a directory */
	if (!acl->is_directory)
		access_mask &= ~NFS4_ACE_DELETE_CHILD;


	ace->access_mask = access_mask & NFS4_ACE_MASK_ALL;

	result = acl_nfs4_set_who(ace, whotype, who);
	if(result < 0)
		return -1;

	TAILQ_INSERT_TAIL(&acl->ace_head, ace, l_ace);
	acl->naces++;

	return 0;
}

