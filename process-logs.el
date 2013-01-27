(defun extract-log-data (buf)
  "Returns a a-list of cutflow and charge measurement"
  (switch-to-buffer buf)
  (search-forward "Inclusive | " nil t nil)
  (kill-word)
  (cons inclusive (string-to-number (yank))))

(defun process-logs (dir suffix) 
  "Iterate over directory containing output logs and summarize the total result of a condor job"
  (mapcar (lambda (file)
	    (when (file-readable-p file) 
	      (extract-log-data (find-file-read-only file))) )
	  (directory-files "/ssh:dmb60@atl008:~/logs/rivet/Sherpa" t "out\\.342")))
