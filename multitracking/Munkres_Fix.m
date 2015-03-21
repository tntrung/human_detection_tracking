% do ham munkers tra ra cac cap nhung chua xet duoi e nguong do do co the
% xot cap duoi nguong khac, thuat toan munkres_fix se lam dieu nay
function [assignment,cost] = Munkres_Fix(costMat,enguong,maxnguong)
%{
%demo 
costMat=[
    1 2 3;
    2 4 6;
    3 6 9;

    ];
enguong=3;
maxnguong=100000;
%}
sizecm=size(costMat);
for c=1:sizecm(1)
    for d=1:sizecm(2)
        if costMat(c,d)>enguong
            costMat(c,d)=maxnguong;
        end
    end
end
%tim cac cap duoi nguong co khoang cach gan nhat
[assignment,cost] = Munkres(costMat);
%xac nhan cac cap nay phai nho hon nguong
for tr=1:sizecm(1)%chay tung tracker
    if assignment(tr)~=0    
        if costMat(tr,assignment(tr))>enguong %neu nho hon nguong
            %huy dong do bang gia tri tren nguong
            assignment(tr)=0;
        end
    end
end