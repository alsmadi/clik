      INTEGER       n,vert1,vert2,i,part,part1,part2
      INTEGER       nodenr,exnodes,expart,edges
      REAL          c
      CHARACTER*15  file
      LOGICAL       filex
      
C     n       is the number of vertices
C     part    is the number of partitions
C     c       is the c in 'c-fat ring'
C     expart  is the last partition that have nodenr+1 nodes
C     exnodes is the number of nodes in partitions 0 to expart
C     edges   is the number of edges
C     file    is the file name where the adjacency matrix will be stored
C     filex   is used to check if the matrix file already exists
C     nodenr  or nodenr+1 is the number of nodes in the partitions
C     vert1   and vert2 is a integer representation of the vectors
C     part1   and part2 is the partitions vert1 vert2 are members of
      
      
C     --- Generate a c-fat ring with n vertices and c*log(n) edges ---
C     ---                   and store it in a file                 ---
      
      write(*,*) 'Please enter c and the number of vertices: '
      read(*,*) c,n
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
         edges = 0
         part = int(n/(c*log(real(n))))
         nodenr = n/part
         expart = mod(n,part)
         exnodes = expart*(nodenr+1)
         write(*,*) 'parts, nodenr, epart, enods:',
     +               part,nodenr,expart,exnodes
         do 10 vert1 = 0,n-2
            part1 = mod(vert1,part)
            do 15 vert2 = vert1+1,n-1
               part2 = mod(vert2,part)
               i = i+1
               if ((abs(part1-part2) .LE. 1) .OR. 
     +             (abs(part1-part2) .EQ. part-1)) then
                  write (25,200,REC=i) .TRUE.
                  edges = edges+1
               else
                  write(25,200,REC=i) .FALSE.
               end if
 200           format(l1)
 15         continue
 10      continue
         write(*,*) 'Number of edges =',edges
         close(25,status='keep')
      end if
      end
