function index=quarter_test(T)
if T(1)==0 && T(2) ==0
    index=0;
elseif T(1)>=0 && T(2)>=0
    index=1;
elseif T(1)>=0 && T(2)<=0
    index=2;    
elseif T(1)<=0 && T(2)<=0
    index=3;    
else 
    index=4;
end