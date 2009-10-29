      LOGICAL   filex
      INTEGER*4 n,vert1,vert2,i
      INTEGER*2 vsize,dist,pos,d
      CHARACTER*15 file
      
C     n       is the number of vertices, n=2**vsize
C     diff    is the hamming difference between vert1 and vert2
C     vsize   is the dimension of the vectors
C     file    is the file name where the adjacency matrix will be stored
C     filex   is used to check if the matrix file already exists
C     i       is used as index in the vectors
C     vert1   and vert2 is a integer representation of the vectors
      
      
C     --- Generate a Hamming graph with vectors of length vsize ---
C     ---                and store it in a file                 ---
      
      write(*,*) 'Please enter length of the binary vectors and the hamm
     +ing distance: '
      read(*,*) vsize,d
      n = 2**vsize
 5    write(*,*) 'Please enter name of the file in which the adjacency m
     +atrix will be stored:'
      read(*,*) file
      inquire(FILE=file, EXIST=filex)
      if (filex) then
         write (*,*) 'The file already exists, delete the existing file
     +or give a new name.'
         write(*,*) ''
         goto 5
      else
         open (UNIT=25,FORM='formatted',FILE=file,ACCESS='direct',
     +        RECL=1,STATUS='new')
         write(25,100,rec=1) n
 100     format(i6)
         i = 6
         do 10 vert1 = 0,n-2
            do 15 vert2 = vert1+1,n-1
               dist = 0
               do 20 pos = 0, vsize-1
                  if (mod(vert1/2**pos,2) .NE. mod(vert2/2**pos,2))
     +                 dist = dist+1
 20            continue
               i = i+1
               if (dist .GE. d) then
                  write (25,200,REC=i) .TRUE.
               else
                  write(25,200,REC=i) .FALSE.
               end if
 200           format(l1)
 15         continue
 10      continue
         close(25,status='keep')
      end if
      end
