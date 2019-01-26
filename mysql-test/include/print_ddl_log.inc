--echo #
--echo # Reading backup ddl log file
--echo #

let MYSQLD_DATADIR= `select @@datadir`;
perl;
  $datadir= $ENV{'MYSQLD_DATADIR'};
  $id_count= 0;
  $tmp_table_count;

  open(FILE, "$datadir/ddl.log") or
    die("Unable to read log file $datadir/ddl.log: $!\n");
  while(<FILE>)
  {
    chop;
    if (/([^\t]*)[\t]([^\t]*)[\t]([^\t]*)[\t]([^\t]*)[\t]([^\t]*)[\t]([^\t]*)[\t]([^\t]*)[\t]([^\t]*)[\t]([^\t]*)[\t]([^\t]*)/)
    {
      # Fix table ids
      $table_id1= "";
      $table_id2= "";
      if (!($6 eq ""))
      {
        $table_id1= "id: $id{$6}";
        if (!exists($id{$6}))
        {
          $id_count++;
          $table_id1= "id: $id_count";
          $id{$6}= $id_count;
        }
      }
      if (!($10 eq ""))
      {
        $table_id2= "id: $id{$10}";
        if (!exists($id{$10}))
        {
          $id_count++;
          $table_id2= "id: $id_count";
          $id{$10}= $id_count;
        }
      }
      # Fix table names
      $table_name1= $5;
      if (substr($table_name1,0,5) eq '@0023')
      {
        if (!exists($name{$table_name1}))
        {
          $tmp_table_count++;
          $name{$table_name1}= "#sql" . $tmp_table_count;
        }
        $table_name1= $name{$table_name1};
      }
      $table_name2= $9;
      if (substr($table_name2,0,5) eq '@0023')
      {
        if (!exists($name{$table_name2}))
        {
          $tmp_table_count++;
          $name{$table_name2}= "#sql" . $tmp_table_count;
        }
        $table_name2= $name{$table_name2};
      }

      print "$2,$3,$4,$table_name1,$table_id1,$7,$8,$table_name2,$table_id2\n";
    }
  }
EOF
