<?php
/*
+----------------------------------------------------------------+
| program/steps/addressbook/delete.inc                           |
|                                                                |
| This file was based on Roundcube Webmail Client                |
| Copyright (C) 2005-2009, RoundCube Dev. - Switzerland          |
|                                                                |
| This file is part of the Crystal Mail Client                   |
| Copyright (C) 2010, Crystal Mail Dev. Team - United States     |
|                                                                |
| Licensed under the GNU GPL                                     |
|                                                                |
| PURPOSE:                                                       |
|   Delete the submitted contacts (CIDs) from the users address  |
|   book                                                         |
|                                                                |
+--------------------- RoundCube Dev. Team ----------------------+
| Thomas Bruederli <roundcube@gmail.com>                         |
+----------------------- Studio 182 Team ------------------------+
| Hunter Dolan <hunter@crystalmail.net>                          |
+----------------------------------------------------------------+
*/


if ($OUTPUT->ajax_call &&
    ($cid = get_input_value('_cid', crystal_INPUT_POST)) &&
    preg_match('/^[a-zA-Z0-9\+\/=_-]+(,[a-zA-Z0-9\+\/=_-]+)*$/', $cid)
) {
  $plugin = $cmail->plugins->exec_hook('contact_delete', array('id' => $cid, 'source' => get_input_value('_source', crystal_INPUT_GPC)));

  $deleted = !$plugin['abort'] ? $CONTACTS->delete($cid) : false;
  if (!$deleted)
    {
    // send error message
    exit;
    }

  // count contacts for this user
  $result = $CONTACTS->count();

  // update message count display
  $OUTPUT->set_env('pagecount', ceil($result->count / $CONTACTS->page_size));
  $OUTPUT->command('set_rowcount', cmail_get_rowcount_text($result->count));

  // add new rows from next page (if any)
  $pages = ceil(($result->count + $deleted) / $CONTACTS->page_size);
  if ($_GET['_from'] != 'show' && $pages > 1 && $CONTACTS->list_page < $pages)
    cmail_js_contacts_list($CONTACTS->list_records(null, -$deleted));

  // send response
  $OUTPUT->send();
  }

exit;

