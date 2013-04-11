USE openray;
INSERT INTO `web_directories` (`request_time`, `name`, `nid`, `public_access`, `state`)SELECT CURRENT_TIMESTAMP , project_projects.uri , project_projects.nid , 'enabled' , 'todo' from project_projects WHERE project_projects.nid NOT IN  (select nid from web_directories);

