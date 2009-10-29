      LOGICAL   diff, kongrt, filex
      INTEGER*4 n, vert1, vert2, i
      INTEGER*2 pos, vsize, sub, comp1, comp2
      CHARACTER*15 file
      
C     n       is the number of vertices, n=4**vsize
C     sub     is the difference between comp1 and comp2
C     vsize   is the dimension of the vectors
C     file    is the file name where the adjacency matrix will be stored
C     pos     is used as index in the vectors
C     filex   is used to check if the matrix file already exists
C     vert1   and vert2 is a integer representation of the vectors
C     diff    and kongr are true if vertices i and j are adjacent
C     comp1   and comp2 are the value of component pos in the vectors
      
      
C     --- Generate a Keller graph with vectors of length vsize ---
C     --- and store it in a file                               ---      
      
      write(*,*) 'Please enter length of Keller vectors: '
      read(*,*) vsize
      n = 4**vsize
 5    write(*,*) 'Please enter name of file in which the adjacency matri
     +x will be stored:'
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
               diff = .FALSE.
               kongrt = .FALSE.
               do 20 pos = 0, vsize-1
                  comp1 = mod(vert1/4**pos,4)
                  comp2 = mod(vert2/4**pos,4)
                  sub = abs(comp1-comp2)
                  if ((sub .eq. 2) .and. (.not. kongrt)) then
                     kongrt = .TRUE.
                  elseif (sub .ne. 0) then
                     diff = .TRUE.
                  end if
 20            continue
               i = i+1
               if (kongrt .and. diff) then
                  write(25,200,REC=i) .TRUE.
               else
                  write(25,200,REC=i) .FALSE.
               end if
 200           format(l1)
 15         continue
 10      continue
         close(25,status='keep')
      end if
      end
