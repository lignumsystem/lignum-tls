%% QSM block output to other formats

% Desc: Script to loop through all files and
%       convert qsm data to other formats
%       using qsmblocks classes
% Input: QSM data
% Output: QSMlignum/, QSMbcyl/, QSMBlender/
% Author: Hannah O'Sullivan


% Clear env
clear

% Add class definitions to path.
addpath('./classes');

% Get directory
dataDir = fullfile("../../OriginalData/",'*.mat');
myFiles = dir(dataDir);

% Loop through all files in data directory 
for k = 1:length(myFiles)
    % Get file names
    baseFileName = myFiles(k).name;
    fullFileName = fullfile(myFiles(k).folder, baseFileName);

    % load file into workspace
    load(fullFileName);

    % Print file name
    disp(baseFileName)

    % Create QSMBcylindrical format
    QSMbcyl = QSMBCylindrical(Sta,Axe,Len,Rad,CPar,SoC,Added);
    
    % Save QSMBcylindrical class for FANNI import
    qsmbcyl_name = sprintf('QSMbcyl/QSMbcyl_%s', baseFileName);
    save(qsmbcyl_name,'QSMbcyl', '-mat')

    % Save QSMBcylindrical for blender import
    minus_mat = erase(baseFileName,".mat");
    blender_name = sprintf('QSMBlender/blender_%s.txt', minus_mat);
     
    QSMbcyl.export( ...
      'blender', ...
       blender_name, ...
       'ColorSource', ...
       'Order' ...
      );
    
    % Save qsmbcylindrical for lignum import
    lignum_name = sprintf('QSMLignum/lignum_%s.txt', minus_mat);
    QSMbcyl.export( ...
    'TXT', ...
    lignum_name ...
);

end
