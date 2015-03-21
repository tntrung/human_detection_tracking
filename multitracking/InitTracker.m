function [track,ID]=InitTracker(region_frame,numrec,ID)
Parameter;                                      %get private parameter
particle=struct('x',0,'y',0,'u',0,'v',0,'w',0); %struct for particles
trackpast=struct('x',0,'y',0);                  %trajectory tracker
sizetrack=zeros(mean_region,2);                 %mean size of the rectangle in frame
track=struct('x',0,'y',0,'width',0,'height',...
0,'cx',0,'cy',0,'u',0,'v',0,'p',particle,...
'use',1,'color',[0 0 0],'tr',trackpast,'st',...
sizetrack,'w',0,'ID',...
0,'positionupdate',zeros(1,2),'HMM',0);             %struct tracker
track(1)=[];
for i=1:numrec
    ID=ID+1;                                            %increase ID of tracker
    track(i).ID=ID;                                     %set ID tracker
    track(i).x=region_frame(i,1);                       %x track
    track(i).y=region_frame(i,2);                       %y track
    track(i).width=region_frame(i,3);                   %width track
    track(i).height=region_frame(i,4);                  %height track    
    track(i).use=2;                                     %init with use track = 2
    track(i).color=[rand(1) rand(1) rand(1)];           %init random color    
    x=region_frame(i,1)+track(i).width/2;               %center x of tracker
    y=region_frame(i,2)+track(i).height/2;              %center y of tracker    
    track(i).tr(1).x=x;                                 %trajectory x tracker save
    track(i).tr(1).y=region_frame(i,2)+track(i).height; %trajectory y tracker save
    track(i).cx=x;                                      %set center x of tracker
    track(i).cy=y;                                      %set center y of tracker
    track(i).u=0;                                       %init vector u with 0
    track(i).v=0;                                       %init vector v with 0        
    track(i).st(1:mean_region,1)=track(i).width;        %set array region mean
    track(i).st(1:mean_region,2)=track(i).height;       %    
    track(i).positionupdate=[0 0];
    if useHMM==1        
        track(i).HMM=IntitHMM([0.2 0.2 0.2 0.2 0.2],[1 1 1 1 1]);
    elseif useHMM==2
        track(i).HMM=IntitHMM([0.2 0.1 0.1 0.1 0.1 0.1 0.1 0.1 0.1],[1 1 1 1 1 1 1 1 1]);
    end
    track(i).unuse=0;
    for j=1:num_particles                               %init for particles
        vec=normrnd(0,1/4,[1 2]);                       %error rate vector
        %estimate vector
        track(i).p(j).u=vec(1);                         %set vector with error rate for particle
        track(i).p(j).v=vec(2);                         %
        posi=normrnd(0,1/4,[1 2]);                      %error rate position
        %estimate position
        track(i).p(j).x=track(i).cx+track(i).p(j).u*numframe_persecond+((track(i).height+track(i).width)/rate_distanttrackpartical)*posi(1);
        track(i).p(j).y=track(i).cy+track(i).p(j).v*numframe_persecond+distant_width_height*((track(i).height+track(i).width)/rate_distanttrackpartical)*posi(2);
        track(i).p(j).w=0;                              %init weight with 0
    end    
end