//
//  ViewController.m
//  game05
//
//  Created by Nishiyama Nobuyuki on 2012/08/19.
//  Copyright (c) 2012年 Nishiyama Nobuyuki. All rights reserved.
//

#define ES1_GL_H_GUARD
#define ES1_GLEXT_H_GUARD
// FIXME:GLKitでES1.1のヘッダを読むのを防ぐ

#import "ViewController.h"
#import "nn_app.hpp"
#import "os_ios.hpp"


using namespace ngs;

@interface ViewController () {
	App *_app;
	Os *_os;
}
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController

- (void)dealloc
{
	delete _app;
	delete _os;
	[_context release];
	[super dealloc];
}

- (void)viewDidLoad
{
	NSLog(@"viewDidLoad");
	
	[super viewDidLoad];
    
	self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2] autorelease];

	if (!self.context) {
		NSLog(@"Failed to create ES context");
	}
    
	GLKView *view = (GLKView *)self.view;
	view.context = self.context;
	view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
	self.preferredFramesPerSecond = 60;
    
	[self setupGL];
	_os = new Os();
	_app = new App(_os->loadPath(), _os->savePath());
}

- (void)viewDidUnload
{    
	NSLog(@"viewDidUnload");

	[super viewDidUnload];
    
	[self tearDownGL];
    
	if ([EAGLContext currentContext] == self.context) {
		[EAGLContext setCurrentContext:nil];
	}
	self.context = nil;
}

- (void)didReceiveMemoryWarning
{
	NSLog(@"didReceiveMemoryWarning");

	[super didReceiveMemoryWarning];
	// Release any cached data, images, etc. that aren't in use.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return YES;
	
	// if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
	// 	return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
	// } else {
	// 	return YES;
	// }
}

- (void)setupGL
{
	NSLog(@"setupGL");
	[EAGLContext setCurrentContext:self.context];

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	// ↑たいてい無視されている模様:P
}

- (void)tearDownGL
{
	NSLog(@"tearDownGL");
	[EAGLContext setCurrentContext:self.context];
}


// 更新
- (void)update
{
	_app->update();
}

// 表示
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	_app->resize(view.drawableWidth, view.drawableHeight);
	_app->draw();
}


@end
