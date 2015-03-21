function [T,rectout,ID]=UpdateWeight(T,D,c_tr_d_score,c_p_tr_class,hog_p_score,ID)
%rectout: -1: init tracker, -2: not update tracker, other: update tracker
Parameter;
if ~isempty(D)
    S=Score(T,D,c_tr_d_score);          %compute score for tracker and detector
    %A=Greedy(S);                        %Connect track-detect
    A=GreedyMunkres(S);                %Connect track-detect with Munkres function
    flag_tr=zeros(1,length(D(:,1)));    %value: 0:init tracker, 1: tracker connect detecter
    rectout=zeros(length(A(1,:))-sum(sum(A))+length(T),4)-1;    
else
    %only update weight for tracker
    flag_tr=1;                          
    lt=length(T);
    if lt==0        
        rectout=zeros(0,4);
        return;
    end    
    A=zeros(length(T),1);
    rectout=zeros(length(T),4)-1;
end
for tr=1:length(T)
    if sum(A(tr,:))==0         
        ntrack=length(T(tr).tr);        
        %delete tracker init
        if ntrack<mintrack
            %T(tr).use=trackvisible-1;
            rectout(tr,:)=[-3 -3 -3 -3];
            continue;
        end 
        T(tr).use=T(tr).use-1;        
        %estimate distant with tracker near        
        maxnear=0;
        for near=1:length(T)
            if sum(A(near,:))
                tempnear=gaussmf(DistantPoint([T(tr).cx T(tr).cy],[T(near).cx T(near).cy]),[sigma_update mean_update]);
                if tempnear>=maxnear
                    maxnear=tempnear;
                end
            end
        end                   
        %estimate HMM
        if useHMM==1	            
            valueindex=EstimateHMM(T(tr).HMM);
            vlp=valueindex(quarter_test([T(tr).p(1).u T(tr).p(1).v])+1);
            vlt=valueindex(quarter_test([T(tr).u T(tr).v])+1);
            if vlp<vlt %thay the vector p1
                T(tr).p(1).u=T(tr).u;
                T(tr).p(1).v=T(tr).v;
            end            
        elseif useHMM==2
            valueindex=EstimateHMM2(T(tr).HMM);
            vlp=valueindex(quarter_test2([T(tr).p(1).u T(tr).p(1).v]));
            vlt=valueindex(quarter_test2([T(tr).u T(tr).v]));
            if vlp<vlt %thay the vector p1
                T(tr).p(1).u=T(tr).u;
                T(tr).p(1).v=T(tr).v;
            end            
        end
        %maxnear=1;
        T(tr).p(1).w = Y*hog_p_score(tr,1)*maxnear + N*c_p_tr_class(tr,1);   
        %estimate HMM
        if useHMM==1	            
            T(tr).p(1).w=T(tr).p(1).w + H*valueindex(quarter_test([T(tr).p(1).u T(tr).p(1).v])+1);            
        elseif useHMM==2
            T(tr).p(1).w=T(tr).p(1).w + H*valueindex(quarter_test2([T(tr).p(1).u T(tr).p(1).v]));            
        end
        maxw=T(tr).p(1).w;
        for p=2:num_particles                                
            T(tr).p(p).w = Y*hog_p_score(tr,p)*maxnear + N*c_p_tr_class(tr,p);
            %estimate HMM
            if useHMM==1
                T(tr).p(p).w=T(tr).p(p).w+ H*valueindex(quarter_test([T(tr).p(p).u T(tr).p(p).v]));
            elseif useHMM==2
                T(tr).p(p).w=T(tr).p(p).w+ H*valueindex(quarter_test2([T(tr).p(p).u T(tr).p(p).v]));
            end
            if maxw<T(tr).p(p).w
                maxw=T(tr).p(p).w;
                tempp=T(tr).p(p);
                T(tr).p(p)=T(tr).p(1);
                T(tr).p(1)=tempp;     
            end
        end        
        T(tr).w=T(tr).p(1).w;   %delete tracker
        %compute and update region mean for tracker
        meanrect=mean([T(tr).st;[T(tr).width T(tr).height]]);
        T(tr).st(1,:)=[];        
        T(tr).st(mean_region,:)=[meanrect(1) meanrect(2)];
        %estimate new position (detector estimate) and update tracker
        x=T(tr).p(1).x-T(tr).cx;
        y=T(tr).p(1).y-T(tr).cy;
        %distant x,y from tracker to detector(update particales)
        T(tr).positionupdate(1)=x;
        T(tr).positionupdate(2)=y;
        %update HMM
        if useHMM==1
            T(tr).HMM=UpdateHMM(T(tr).HMM,0.5,quarter_test(T(tr).positionupdate));
        elseif useHMM==2
            T(tr).HMM=UpdateHMM2(T(tr).HMM,0.5,quarter_test2(T(tr).positionupdate));
        end        
        T(tr).cx=T(tr).cx+x;
        T(tr).cy=T(tr).cy+y;        
        T(tr).x=max(1,T(tr).cx-meanrect(1)/2);
        T(tr).y=max(1,T(tr).cy-meanrect(2)/2);
        T(tr).width=meanrect(1);
        T(tr).height=meanrect(2);     
        T(tr).tr(ntrack+1).x=T(tr).cx;
        T(tr).tr(ntrack+1).y=T(tr).y+meanrect(2); 
        rectout(tr,:)=[-2 -2 -2 -2];                %label update tracker (unupdate)
    else
        for d=1:length(D(:,1))
            if A(tr,d)==1
                flag_tr(1,d)=1;
                 %estimate HMM
                if useHMM==1
                    valueindex=EstimateHMM(T(tr).HMM);
                    vlp=valueindex(quarter_test([T(tr).p(1).u T(tr).p(1).v])+1);
                    vlt=valueindex(quarter_test([T(tr).u T(tr).v])+1);
                    if vlp<vlt %thay the vector p1
                        T(tr).p(1).u=T(tr).u;
                        T(tr).p(1).v=T(tr).v;
                    end                   
                elseif useHMM==2
                    valueindex=EstimateHMM2(T(tr).HMM);
                    vlp=valueindex(quarter_test2([T(tr).p(1).u T(tr).p(1).v]));
                    vlt=valueindex(quarter_test2([T(tr).u T(tr).v]));
                    if vlp<vlt %thay the vector p1
                        T(tr).p(1).u=T(tr).u;
                        T(tr).p(1).v=T(tr).v;
                    end                    
                end
                %T(tr).p(1).w = B*gaussmf(DistantPoint([T(tr).p(1).x T(tr).p(1).y],[D(d,1)+D(d,3)/2 D(d,2)+D(d,4)/2]),[sigma_update mean_update]) + Y*hog_p_score(tr,1) + N*c_p_tr_class(tr,1);
                T(tr).p(1).w=B*gaussmf(...
                        DistanceVector(...
                            [T(tr).p(1).x T(tr).p(1).y T(tr).p(1).u T(tr).p(1).v],...
                            [D(d,1)+D(d,3)/2 D(d,2)+D(d,4)/2]...
                            ),[sigma_update mean_update]...
                    )+Y*hog_p_score(tr,1)+N*c_p_tr_class(tr,1);
                %estimate HMM
                if useHMM==1                    
                    T(tr).p(1).w=T(tr).p(1).w+ H*valueindex(quarter_test([T(tr).p(1).u T(tr).p(1).v])+1);
                elseif useHMM==2                    
                    T(tr).p(1).w=T(tr).p(1).w+ H*valueindex(quarter_test2([T(tr).p(1).u T(tr).p(1).v]));
                end
                maxw=T(tr).p(1).w;                                                  
                for p=2:num_particles                    
                    %T(tr).p(p).w=B*gaussmf(DistantPoint([T(tr).p(p).x T(tr).p(p).y],[D(d,1)+D(d,3)/2 D(d,2)+D(d,4)/2]),[sigma_update mean_update]) + Y*hog_p_score(tr,p) + N*c_p_tr_class(tr,p) ;
                    T(tr).p(p).w=B*gaussmf(...
                        DistanceVector(...
                            [T(tr).p(p).x T(tr).p(p).y T(tr).p(p).u T(tr).p(p).v],...
                            [D(d,1)+D(d,3)/2 D(d,2)+D(d,4)/2]...
                            ),[sigma_update mean_update]...
                    )+Y*hog_p_score(tr,p)+N*c_p_tr_class(tr,p);
                    if useHMM==1
                        T(tr).p(p).w=T(tr).p(p).w+ H*valueindex(quarter_test([T(tr).p(p).u T(tr).p(p).v])+1);
                    elseif useHMM==2
                        T(tr).p(p).w=T(tr).p(p).w+ H*valueindex(quarter_test2([T(tr).p(p).u T(tr).p(p).v]));
                    end
                    if maxw<T(tr).p(p).w                        
                        maxw=T(tr).p(p).w;
                        tempp=T(tr).p(p);
                        T(tr).p(p)=T(tr).p(1);
                        T(tr).p(1)=tempp;                                                                    
                    end
                end                             
                %distant x,y from tracker to detector(update particales)
                T(tr).positionupdate(1)=D(d,1)+D(d,3)/2-T(tr).cx;
                T(tr).positionupdate(2)=D(d,2)+D(d,4)/2-T(tr).cy;
                %update HMM
                if useHMM==1
                    T(tr).HMM=UpdateHMM(T(tr).HMM,0.5,quarter_test(T(tr).positionupdate)+1);
                elseif useHMM==2
                    T(tr).HMM=UpdateHMM2(T(tr).HMM,0.5,quarter_test2(T(tr).positionupdate));
                end
                %compute and update region mean for tracker
                meanrect=mean([T(tr).st;[D(d,3) D(d,4)]]);                                
                T(tr).st(1,:)=[];                
                T(tr).st(mean_region,:)=[meanrect(1) meanrect(2)];
                %update tracker with new detect
                T(tr).cx=D(d,1)+D(d,3)/2;
                T(tr).cy=D(d,2)+D(d,4)/2;                
                T(tr).x=max(1,T(tr).cx-meanrect(1)/2);
                T(tr).y=max(1,T(tr).cy-meanrect(2)/2);
                T(tr).width=meanrect(1);
                T(tr).height=meanrect(2);                
                T(tr).use=1;  
                ntrack=length(T(tr).tr);
                T(tr).tr(ntrack+1).x=T(tr).cx;
                T(tr).tr(ntrack+1).y=T(tr).y+meanrect(2);
                T(tr).u=T(tr).p(1).u;            
                T(tr).v=T(tr).p(1).v;
                T(tr).w=T(tr).p(1).w;                       
                rectout(tr,:)=[D(d,2) D(d,1) D(d,4) D(d,3)];                
                break;            
            end
        end
    end    
end
if flag_tr==1
    return;     %not exists detector miss
end
%init tracker for detector miss
if (isempty(T)) 
    tr=0; 
end
for i=1:length(flag_tr)    
    if flag_tr(1,i)==0      %detector miss
        tr=tr+1;      
        [T(tr),ID]=InitTracker(D(i,:),1,ID);        
    end
end    