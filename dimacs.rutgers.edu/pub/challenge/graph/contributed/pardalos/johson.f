      PARAMETER (MAXL = 30)
      LOGICAL   filex,moved,equal
      INTEGER   n,vert1,vert2,i,vsize,dist,one,d,w,edges
      INTEGER   index1(MAXL),index2(MAXL),choose
      CHARACTER*15 file
      
C     n       is the number of vertices, n = vsize choose w
C     w       is the weight of the binary vectors
C     d       is the desired hamming distance
C     dist    is the hamming difference between vert1 and vert2
C     vsize   is the dimension of the vectors
C     file    is the file name where the adjacency matrix will be stored
C     i       is used to count records while printing to file
C     filex   is used to check if the matrix file already exists
C     index1  is used to store the indices of the ones in vert1
C     index2  is used to store the indices of the ones in vert2
C     one     is used as an index in index1 and index2
C     vert1   and vert2 is an integer representation of the vectors
      
      
C     --- Generate a Johnson graph with vectors of length vsize,  ---
C     --- weight w and hamming distance, and store it in a file.  ---
      
      write(*,*) 'Please enter length, weight and hamming distance.'
      read(*,*) vsize,w,d
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
         n = choose(vsize,w)
         write(*,*) 'Number of nodes =',n
         write(25,100,rec=1) n
 100     format(i6)
         i = 6
         
C        --- Initialize index1 and index2. ---
         
         do 10 one = 1,w+1
            index1(one) = vsize+1
            index2(one) = vsize+1
 10      continue

C        --- Create the graph and write it to a file. ---
         
         do 20 vert1 = 1,n-1
            moved = .FALSE.
            do 30 one = 1,w
               if (.NOT. moved) then
                  if (index1(one) .LT. (index1(one+1)-1)) then

C                 --- Move one nr 'one' to the left. ---
                     
                     index1(one) = index1(one)+1
                     moved = .TRUE.
                  else

C                 --- Move one nr. 'one' to index 'one'. ---
                     
                     index1(one) = one
                  endif
               endif
               index2(one) = index1(one)
 30         continue
            do 40 vert2 = vert1+1,n 
               dist = 0
               moved = .FALSE.
               do 50 one = 1,w
                  if (.NOT. moved) then
                     if (index2(one) .LT. (index2(one+1)-1)) then

C                    --- Move one nr 'one' to the left. ---
                     
                        index2(one) = index2(one)+1
                        moved = .TRUE.
                     else

C                    --- Move one nr. 'one' to index 'one'. ---
                     
                        index2(one) = one
                     endif
                  endif
                  equal = .FALSE.
                  do 55 m =1,w
                     if (index2(one) .EQ. index1(m)) equal = .TRUE.
 55               continue

C                 --- If not ones in the same position, ---
C                 ---    distance is increased by 2.    ---
                     
                  if (.NOT. equal) dist = dist+2
 50            continue
               i = i+1
               if (dist .GE. d) then

C              --- If distance is greater than d, ---
C              --- vert1 and vert2 are adjacent.  ---
                  
                  write(25,200,REC=i) .TRUE.
                  edges = edges+1
               else
                  write(25,200,REC=i) .FALSE.
               end if
 200           format(l1)
 40         continue
 20      continue
         write(*,*) 'Number of edges =',edges
         close(25,status='keep')
      end if
      end
      
      
      INTEGER FUNCTION choose (n,w)
      INTEGER n,w
      INTEGER i,k
      if (n .LT. w) then
         choose = 0
         return
      else
         i = n
         do 10 k = 1,w-1
            i = i*(n-k)/(k+1)
 10      continue
         choose = i
      endif
      return
      end

