function [gt,x,y]=GetIndexMax(S)
[gt,x]=max(S);
[gt,y]=max(gt);
x=x(y);