/* Copyright 2003-2017 GBDI-ICMC-USP <caetano@icmc.usp.br>
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* 
* 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**
* @file
*
* This file implements the VPTree node.
*
* @version 1.0
* @author Ives Renê Venturini Pola (ives@icmc.usp.br)
*/
#include <arboretum/stVPNode.h>

//-----------------------------------------------------------------------------
// class stVPNode
//-----------------------------------------------------------------------------
stVPNode::stVPNode(stPage * page, bool create){

   this->Page = page;

   // if create is true, we must to zero fill the page
   if (create){
      Page->Clear();
   }//end if

   // Set elements
   this->Header = (stVPNodeHeader *) this->Page->GetData();
   this->Entry = (u_int32_t *)(this->Page->GetData() + sizeof(stVPNodeHeader));
}//end stVPNode::stVPNode

//------------------------------------------------------------------------------
bool stVPNode::AddEntry(u_int32_t size, const unsigned char * object){
   u_int32_t totalsize;
   u_int32_t offs;

   totalsize = size + sizeof(u_int32_t);
   if (totalsize <= GetFree()){

      offs = Page->GetPageSize() - size;

      // Update entry offset
      *Entry = offs;
      // Write object
      memcpy( (void *) (Page->GetData() + *Entry), (void *) object, size);

      return true;
   }else{
      // there is no room for the object
      return false;
   }//end if
}//end stVPNode::AddEntry

//------------------------------------------------------------------------------
const unsigned char * stVPNode::GetObject(){
   return (unsigned char *) Page->GetData() + *Entry;
}//end stVPNode::GetObject

//------------------------------------------------------------------------------
u_int32_t stVPNode::GetObjectSize(){
   return (u_int32_t) Page->GetPageSize() - *Entry;
}//end stVPNode::GetObjectSize

//------------------------------------------------------------------------------
u_int32_t stVPNode::GetFree(){
   return Page->GetPageSize() - sizeof(stVPNodeHeader);
}//end stVPNode::GetFree
