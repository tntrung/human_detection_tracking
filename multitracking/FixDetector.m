function d=FixDetector(d)
%d is ouput getdetector, function fix d to format x,y,w,h
if size(d)==0                   %if d empty
    d=zeros(0,4);               %return empty
    return;                     %exit function
end
n=length(d(:,1));               %get num d
for i=n:-1:1
    if d(i,:)==0
        %delete for message from GetDetector.m
        d(i,:)=[];
    else
        %converter format [x,y,w,h]
        d(i,3)=d(i,3)-d(i,1);
        d(i,4)=d(i,4)-d(i,2);
    end
end