%% script to test leaf addition with wytham data

% Clear environment
clear

%% Include class definitions.
addpath('classes/');

%% Import data

%  Test tree
load("QSMbcyl/QSMbcyl_wytham_winter_5a-0.2-0.22-4-0.04-0.044-3-5-1-0")

%% Define leaf shape.

% Vertices of the leaf basis geometry.
vertices = [
    -0.3  0.0  0.0;
    -0.3  1.0  0.0;
     0.3  1.0  0.0;
     0.3  0.0  0.0
];

% Triangles of the leaf basis geometry.
tris = [
     1,  2,  3;...
     1,  3,  4
];

%% Leaf insertion.

% Genereate 300 m2 of leaf candidates,
% stop if 100 m2 of leaf area is accepted.
LeafArea = [100,300];

% Initialize the leaf model with the basis geometry.
Leaves = LeafModelTriangle(vertices, tris, {[1 2 3 4]});

% Generate leaves.
[Leaves, NAccepted] = qsm_fanni( ...
    QSMbcyl,...
    Leaves,...
    LeafArea,...
    'Seed',1,...
    'SizeFunctionParameters', {[0.25 0.30]},...
    'Verbose',true ...
);

% Optional steps to follow:

%% Plot results.

% Plot QSM.
hQSM = QSMbcyl.plot_model();
% Set bark color.
set(hQSM,'FaceColor',[150,100,50]./255,'EdgeColor',[0 0 0]);

hold on;

% Plot leaves.
hLeaf = Leaves.plot_leaves();
% Set leaf color.
set(hLeaf,'FaceColor',[120,150,80]./255);

hold off;
axis equal;

%% Compute geometry of accepted leaves.

[Vertices, Faces] = Leaves.compute_geometry(false);

%% Export result.

% Use ngons when exporting leaves.
fUseNgon = true;

% Export in OBJ-format with individual leaf vertices and faces.
Leaves.export_geometry( ...
    'OBJ', ...
    fUseNgon, ...
    'ww_leaves/ww_test_tree_blender_leaves_export.obj', ...
    4 ...
);

% Export in custom extended text format with basis leaf geometry 
% and individual leaf transformation parameters.
Leaves.export_geometry( ...
    'txt', ...
    fUseNgon, ...
    'ww_leaves/ww_test_tree_leaves_export_extended.txt', ...
    4 ...
);

% Export QSM parameters to a text file.
QSMbcyl.export( ...
    'blender', ...
    'ww_stems/ww_blender_test_tree_.txt', ...
    'Precision',4 ...
);
