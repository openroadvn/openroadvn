<?php // $Id: page.tpl.php,v 0.00.5 2011/01/19 09:57:31 sebastien.millart Exp $ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language -> language ?>" lang="<?php print $language -> language ?>" dir="<?php print $language -> dir ?>">
<head>
<title><?php print $head_title; ?></title>
<?php  $theme_path = '/sites/all/themes/joinup'; ?>
<link type="image/x-icon" href="<?php print $theme_path; ?>/images/logo/favicon.ico" rel="shortcut icon">
<link type="text/css" rel="stylesheet" media="all" href="<?php print $theme_path; ?>/styles/reset.css" />
<link type="text/css" rel="stylesheet" media="all" href="<?php print $theme_path; ?>/styles/grid.css" />
<link type="text/css" rel="stylesheet" media="all" href="<?php print $theme_path; ?>/styles/common.css" />
<link type="text/css" rel="stylesheet" media="all" href="<?php print $theme_path; ?>/styles/style.css" />
<link type="text/css" rel="stylesheet" media="all" href="<?php print $theme_path; ?>/styles/forms.css" />
<link type="text/css" rel="stylesheet" media="all" href="<?php print $theme_path; ?>/styles/dashboard.css" />
<link type="text/css" rel="stylesheet" media="print" href="<?php print $theme_path; ?>/styles/print.css" />
<link type="text/css" rel="stylesheet" media="all" href="/modules/system/maintenance.css" />
</head>
<body>
	<div class="accessibility-info">
		<h2>Accessibility tools</h2>
        <?php print 'test ' . $theme_path; ?>
		<ul>
			<li><a href="#content" accesskey="S"><?php print t('Go to main content [shortcut key S], by skipping navigation top bar, search box and navigation menu.'); ?></a></li>
			<li><a href="#block-menu-primary-links"><?php print t('Go to navigation menu, by skipping navigation top bar and search box.'); ?></a></li>
			<li><a href="#block-search-0"><?php print t('Go to search box, by skipping navigation top bar.'); ?></a></li>
			<li><a href="#block-user-1"><?php print t('Go to navigation top bar.'); ?></a></li>
			<li><a href="#block-quick-actions"><?php print t('Go to quick actions links, by skipping navigation top bar, search box, navigation menu and main content.'); ?></a></li>
			<li><a href="#block-menu-secondary-links"><?php print t('Go to footer menu, by skipping navigation top bar, search box, navigation menu, main content and quick actions links.'); ?></a></li>
		</ul>
	</div>
	<div id="container">
		<div id="ec-banner-wrapper">
			<div id="ec-banner" class="grid16col clearfix">
				<div class="colspan-16">
					<div id="block-menu-secondary-links">
						<?php if (isset($secondary_links)) : print theme_links($secondary_links); endif; ?>
					</div>
					<span id="ec-banner-image-title"><?php print t('European Commission'); ?> | <?php print t('Interoperability Solutions for European Public Administrations'); ?></span>
					<span id="ec-banner-image-right">&nbsp;</span>
				</div>
			</div>
		</div>
		<div id="breadcrumb-wrapper">
			<div id="breadcrumb" class="grid16col clearfix">
				<div class="colspan-16">
					<h2 class="accessibility-info">Breadcrumb</h2>
					<p><strong><a href=""></a> &rsaquo; <a href=""></a> &rsaquo; &rsaquo; <?php if ($breadcrumb): print $breadcrumb; endif; ?></strong></p>
				</div>
			</div>
		</div>
		<div id="header-wrapper">
			<div id="header" class="grid16col clearfix">
				<div id="header-region" class="colspan-16">
					<div id="logo-floater">
							<h1>
<img src="<?php print $theme_path; ?>/images/logo/joinup.png" alt="Joinup logo" title="" id="logo">
</h1>
					</div>
					<?php if ($header): print $header; endif; ?>
					<div id="block-menu-primary-links">
						<?php if (isset($primary_links)) : print theme_links($primary_links); endif; ?>
					</div>
				</div>	<!-- /#header-region -->
			</div>
		</div>
		<div id="highlight-wrapper">
			<?php if ($highlight): ?>
				<div id="highlight" class="grid16col clearfix">
					<?php if ($highlight): ?>
						<div id="highlight-region" class="colspan-16">
							<?php print $highlight; ?>
    						</div>	<!-- /#highlight-region -->
					<?php endif; ?>
				</div>
			<?php else: ?>
				<div class="empty">&nbsp;</div>
			<?php endif; ?>
		</div>
		<div id="content-wrapper" class="grid16col clearfix">
			<?php if ($left): ?>
				<div id="left-sidebar-region" class="colspan-<?php print $left_colspan; ?>">
					<?php print $left;?>
				</div>	<!-- /#left-sidebar-region -->
			<?php endif; ?>
			<div id="content" class="colspan-<?php print $content_colspan; ?>">
				<?php if ($show_messages && $messages): print $messages; endif; ?>
				<?php if ($tabs): ?>
					<div id="tabs-wrapper">
						<?php print $tabs ?>
					</div>
				<?php endif; ?>
				<?php if ($before_content): ?>
					<div id="before-content-region" class="clearfix">
						<?php print $before_content ?>
					</div>	<!-- /#before-content-region -->
				<?php endif; ?>
				<?php if ($title): ?>
					<h2><?php print $title ?></h2>
				<?php endif; ?>
				<?php print $help; ?>
				<div id="content-region">
					<?php print $content ?>
				</div>	<!-- /#content-region -->
				<?php if ($after_content): ?>
					<div id="after-content-region" class="clearfix">
						<?php print $after_content ?>
					</div>	<!-- /#after-content-region -->
				<?php endif; ?>
			</div>
			<?php if ($right): ?>
				<div id="right-sidebar-region" class="colspan-<?php print $right_colspan; ?>">
					<?php print $right; ?>
				</div>	<!-- /#right-sidebar-region -->
			<?php endif; ?>
		</div>
		<div id="footer" class="grid16col clearfix">
			<div id="footer-region" class="colspan-16">
				<?php // print $feed_icons ?>
				<?php print $footer ?>
			</div>	<!-- /#footer-region -->
		</div>
	</div>	<!-- /#container -->
	
	<?php print $closure; ?>
	
</body>
</html>
