      subroutine getttimes(phlst,depth,delta,pn,tt,dtdd,dtdh,dddp,phcd)
        
        implicit real(a-h,o-z)
        parameter (max=60)
        logical prnt(3)
        character*8 phcd(max),phlst(max)
        character*64 modnam
        real depth
        integer ix,nb,slen,pn
        dimension tt(max),dtdd(max),dtdh(max),dddp(max),mn(max),ts(max)
        dimension usrc(2)
        data in/1/,prnt(3)/.true./
        include 'modnam.inc'

        ix = indexr(modnam,'/')+1
        prnt(1) = .false.
        prnt(2) = .false.

        call assign(10,2,'/tmp/ttim1.lis')
        call tabin(in,modnam,prnt)
        call brnset(pn,phlst,prnt)
        
        zs = depth
        call depset(zs,usrc)
        
        call trtm(delta,max,n,tt,dtdd,dtdh,dddp,phcd)
        pn = n
      end

      integer function indexr(string,ch)
        character string*(*), ch*(*)

        lch = len(ch)
        do 1 i=len(string)-lch,1,-1
            if (string(i:i+lch-1) .eq. ch) go to 10
1       continue
        i = 0
10      continue
        indexr = i
      end
