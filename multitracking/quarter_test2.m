function index=quarter_test2(T)
Parameter;
if T(1)==0 && T(2) ==0
    index=1; %central
elseif T(1)>=0 && T(2)>=0
    if T(1)<T(2)
        index=2;
    else
        index=3;
    end
elseif T(1)>=0 && T(2)<=0
    if T(1)>T(2)*-1
        index=4;
    else
    	index=5;
    end
elseif T(1)<=0 && T(2)<=0
    if T(1)>T(2)
        index=6;
    else
        index=7;
    end    
else %if T(1)<0 && T(2)<0
    if T(1)*-1>T(2)
        index=8;
    else
        index=9;
    end
end