function S=Score(T,D,c_tr_d_score)
Parameter;
if isempty(T) || isempty(D)         %return if Tracker or detector isempty
    S=0;
    return;
end
S=zeros(length(T),length(D(:,1)));  %matrix score
sizeS=size(S);
for tr=1:sizeS(1)
    for d=1:sizeS(2)        
        sumgauss=0;                 %sum score        
        for itr=1:num_particles            
            %compute distant partical itr to detector
            distdp=DistantPoint([T(tr).p(itr).x T(tr).p(itr).y],[D(d,1)+D(d,3)/2 D(d,2)+D(d,4)/2]);                     
            sumgauss=sumgauss+gaussmf(distdp,[sigma_score mean_score]);
        end
        %disp('ddddddddddddddddddddddddddddddddddddddddddddd');
        %disp(GatingFunction(T(tr),D(d,:)));
        %disp(c_tr_d_score(tr,d));
        %disp(sumgauss);
        %disp('ddddddddddddddddddddddddddddddddddddddddddddd');
        S(tr,d)=GatingFunction(T(tr),D(d,:))*(c_tr_d_score(tr,d)+(1/ampha_score)*sumgauss);        
    end
end