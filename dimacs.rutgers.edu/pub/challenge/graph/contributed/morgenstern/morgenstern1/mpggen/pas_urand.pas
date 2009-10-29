{$nomain}
{$case}
{$double}

%include 'urand.h';

procedure init_urand;
   begin
     _init_urand
   end;

function urand;
   begin
     _urand(y);
     urand := y / maxint
   end;
     
