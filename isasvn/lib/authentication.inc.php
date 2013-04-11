<?php
class Authentication {

	private $login;
	private $passwd;
	private $proj;
	private $usr_pwd_proj;
	private $local_server;
	private $local_db_username;
	private $local_db_password;
	private $local_db_name;
	private $isa_server;
	private $isa_db_username;
	private $isa_db_password;
	private $isa_db_name;

	public function __construct($local_server, $local_db_username,
		$local_db_password, $local_db_name, $isa_server, $isa_db_username,
		$isa_db_password, $isa_db_name)
	{
		$this->login = getenv('USER');
		$this->passwd = getenv('PASS');
		$this->proj = $this->extractProjectFromURI(getenv('URI'));
		$this->usr_pwd_proj = md5($this->login . $this->passwd . $this->proj);
		$this->local_server = $local_server;
		$this->local_db_username = $local_db_username;
		$this->local_db_password = $local_db_password;
		$this->local_db_name = $local_db_name;
		$this->isa_server = $isa_server;
		$this->isa_db_username = $isa_db_username;
		$this->isa_db_password = $isa_db_password;
		$this->isa_db_name = $isa_db_name;
	}

	public function __get($var) {
		return $this->$var;
	}

	public function __set($var, $value) {
		$this->$var = $value;
	}

	protected function extractProjectFromURI($uri) {
		$uri = trim($uri, '/');

		$uri_prefix = defined('SVN_URI_PREFIX') ? constant('SVN_URI_PREFIX') : '/svn';
		$uri_prefix = trim($uri_prefix, '/');
		if (strlen($uri_prefix)) {
			$uri = preg_replace('/^' . $uri_prefix . '/', '', $uri, 1);
			$uri = ltrim($uri, '/');
		}

		$uri_parts = explode('/', $uri);
		return $uri_parts[0];
	}

	public function set_authenticated() {
		$connection = mysql_connect($this->local_server, $this->local_db_username,
			$this->local_db_password);
		if ($connection)
		{
			if (mysql_select_db($this->local_db_name, $connection))
			{
				// Already authenticated at least once ?
				$sql = sprintf(
					'SELECT usr_pwd_proj
					FROM auth_cache
					WHERE usr_pwd_proj = \'%s\'',
					mysql_real_escape_string($this->usr_pwd_proj)
				);
				$req = mysql_query($sql)
						or die('Erreur SQL !<br />'.mysql_error());
				$record = mysql_fetch_assoc($req);
				if (!$record)
				{
					// Never authenticated
					$sql = sprintf(
						'INSERT INTO auth_cache (last_auth, usr_pwd_proj)
						VALUES (UNIX_TIMESTAMP(), \'%s\')',
						mysql_real_escape_string($this->usr_pwd_proj)
					);
					$req = mysql_query($sql)
						or die('Erreur SQL !<br />'.mysql_error());
				}
				else
				{
					// Already authenticated at least once
					$sql = sprintf(
						'UPDATE auth_cache
						SET last_auth = UNIX_TIMESTAMP()
						WHERE usr_pwd_proj = \'%s\'',
						mysql_real_escape_string($this->usr_pwd_proj)
					);
					$req = mysql_query($sql)
						or die('Erreur SQL !<br />'.mysql_error());
				}
			}
		}
	}

	protected function getSVNAuthCacheLifetime() {
		return defined('SVN_AUTH_CACHE_LIFETIME') ? constant('SVN_AUTH_CACHE_LIFETIME') : 600;
	}

	public function check_authenticated() {
		$connection = mysql_connect($this->local_server, $this->local_db_username,
			$this->local_db_password);
		if ($connection)
		{
			if (mysql_select_db($this->local_db_name, $connection))
			{
				// Check if authenticated in the last 10 minutes (600 seconds)
				$auth_cache_lifetime = $this->getSVNAuthCacheLifetime();
				$sql = sprintf(
					'SELECT usr_pwd_proj
					FROM auth_cache
					WHERE usr_pwd_proj = \'%s\'
					AND (UNIX_TIMESTAMP() - last_auth) < %d',
					mysql_real_escape_string($this->usr_pwd_proj),
					$auth_cache_lifetime
				);
				$req = mysql_query($sql)
						or die('Erreur SQL !<br />'.mysql_error());
				$record = mysql_fetch_assoc($req);
				if (!$record)
					return false;
				return true;
			}
		}
		return false;
	}

	public function authenticate() {
		$connection = mysql_connect($this->isa_server, $this->isa_db_username,
			$this->isa_db_password);
		if ($connection)
		{
			if (mysql_select_db($this->isa_db_name, $connection))
			{
				// Modify the permission depending on the context of the call
				$perm = defined( 'AUTH_CONTEXT' ) && constant('AUTH_CONTEXT') == 'svn' ? 
					'access source code repository' :
					'access webdav repository';
				
				// authenticates owner of project and
				// users with correct role
				$sql = sprintf(
					'SELECT u.uid
						FROM og_uid ou
						INNER JOIN users u ON u.uid = ou.uid
						INNER JOIN node n ON n.nid = ou.nid
						INNER JOIN project_projects pp ON pp.nid = ou.nid
						LEFT JOIN og_users_roles our ON our.uid = ou.uid AND ou.nid = our.gid
						LEFT JOIN permission p ON p.rid = our.rid
					WHERE
						u.name = \'%s\'
						AND u.pass = MD5(\'%s\')
						AND pp.uri = \'%s\'
						AND (
							p.perm LIKE \'%%%s%%\'
							OR n.uid = u.uid
						)',
					mysql_real_escape_string($this->login),
					mysql_real_escape_string($this->passwd),
					mysql_real_escape_string($this->proj),
					$perm
				);
				$req = mysql_query($sql)
					or die('Erreur SQL !<br />'.mysql_error());
				$uid = mysql_fetch_assoc($req);
				if (!$uid)
					return 1;
				$this->set_authenticated();
				return 0;
			}
		}
		return 1;
	}

	function exit_auth_status() {
		if ($this->proj == "") {
			exit(1);
		}
		if ($this->check_authenticated())
			exit(0);
		exit($this->authenticate());
	}

}
