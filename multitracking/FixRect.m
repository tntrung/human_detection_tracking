function [c,u]=FixRect(a,b)
%a,b is rect with parameter two point x1,y1 and x2,y2
c=zeros(1,4);                   %c the intersection of two rectangles [x1,y1;x2,y2]
for i=1:2    
    c(i)=max(a(i),b(i));        %max left,upper
    c(i+2)=min(a(i+2),b(i+2));  %max right,bottom
end
u=1;%test c valid
if c(1)>=c(3)||c(2)>=c(4)       %c invalid
    u=0;                        %delete rect message
end