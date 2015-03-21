function region=GetDetector(link,h,w,numframe)
fid = fopen(link, 'r'); %open file output from detector
A = fscanf(fid, '%f %f %f %f %f %f %f %f %f %f',[10 inf]); %f %f %f %f %f %f %f %f %f %f %f %f %f', [23 inf]);
fclose(fid);            %close file
sizea=size(A);
region=struct('rec',zeros(0,4));
for i=1:sizea(2)        %get rown
    x=A(7,i);           %x
    y=A(8,i);           %y     
    wx=A(9,i);          %width   
    wh=A(10,i);         %height  
    [region(A(1,i)+1).rec(A(2,i)+1,:),u]=FixRect([x y wx+x wh+y],[1 1 w-1 h-1]);%fix rect in whole image
    if u==0             %if detector don-t exits in wholeimage    
        region(A(1,i)+1).rec(A(2,i)+1,:)=[0 0 0 0];%delete message for FixDetector.m
    end    
end
if length(region)<numframe
    for i=length(region)+1:numframe
        region(i).rect=[0 0 0 0];
    end
end