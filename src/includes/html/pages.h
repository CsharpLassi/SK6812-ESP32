#ifndef __PAGES_H__
#define __PAGES_H__

const char *indexPage = " <html> \
                            <head> \
                              <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\">  \
                            </head> \
                            <body> \
                            <div class=page> \
                              <div class=\"vertical-center\" style=\"min-height: 100%%; display: flex; align-items: center;\"> \
                                <div class=\"container\"> \
                                  <div class=\"row justify-content-sm-center\"> \
                                    <div class=\"col-sm-12 col-md-3\"> \
                                      <div class=\"card\"> \
                                        <h4 class=\"card-header\">Commands</h4>\
                                        <div class=\"card-body\"> \
                                          <a href=\"/on\" class=\"nav-link\">On</a> \
                                          <a href=\"/off\" class=\"nav-link\">Off</a> \
                                        </div> \
                                      </div> \
                                    </div> \
                                    <div class=\"col-sm-12 col-md-9\"> \
                                      <div class=\"card\"> \
                                        <h4 class=\"card-header\">Background</h4>\
                                        <div class=\"card-body\"> \
                                          <form action=\"/option\" method=\"get\"> \
                                            <div class=\"form-group\"> \
                                              <label for=\"backgroundColor\">Color:</label> \
                                              <input type=\"color\" class=\"form-control\" id=\"backgroundColor\" name=\"backgroundColor\" > \
                                            </div> \
                                            <button type=\"submit\" class=\"btn btn-primary\">Submit</button> \
                                          </form> \
                                        </div> \
                                      </div> \
                                    </div> \
                                  </div> \
                                </div> \
                              </div> \
                            </div> \
                            </body> \
                          </html> \
                          ";

const char *redirectPage = " <html> \
                            <head> \
                              <meta http-equiv=\"refresh\" content=\"0; url=/index \" /> \
                              <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\">  \
                            </head> \
                            <body> \
                            <div class=page> \
                              <div class=\"vertical-center\" style=\"min-height: 100%%; display: flex; align-items: center;\"> \
                                <div class=\"container\"> \
                                  <div class=\"row justify-content-sm-center\"> \
                                  <a href=\"/index\" class=\"nav-link\">Redirect</a> \
                                  </div> \
                                </div> \
                              </div> \
                            </div> \
                            </body> \
                          </html> \
                          ";


#endif
