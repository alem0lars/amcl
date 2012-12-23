require 'pathname'
require 'fileutils'
require 'rake/clean'


ROOT_PTH = Pathname.new Dir.getwd
# { Configuration Variables
SRC_PTH = ROOT_PTH.join 'src'
SRC_MAIN_FILE_PTH = SRC_PTH.join 'main.c'
TEST_PTH = ROOT_PTH.join 'test'
OUT_PTH = ROOT_PTH.join 'out'
DOC_PTH = OUT_PTH.join 'doc'
OBJ_PTH = OUT_PTH.join 'obj'
TMP_PTH = ROOT_PTH.join 'tmp'
DOXYGEN_CONFIG_FILE_PTH = ROOT_PTH.join '.doxygen_config'
DOC_HTML_ALLOWED = true
DOC_LATEX_ALLOWED = true
DOC_MAN_ALLOWED = true
APP_NAME = File.basename ROOT_PTH
APP_FILE_PTH = OUT_PTH.join APP_NAME
TEST_FILE_PTH = OUT_PTH.join 'run_tests'
# }

SRC = FileList[SRC_PTH.join('**').join('*.c').to_s]
SRC_TEST = FileList[TEST_PTH.join('**').join('*.c').to_s]
OBJ = SRC.collect { |fn| File.join(OBJ_PTH, File.basename(fn).ext('o')) }


# { Utility functions
def find_source(obj_file)
  file_name = File.basename(obj_file, '.o')
  SRC.find { |src_elem| File.basename(src_elem, '.c') == file_name }
end
def subpaths(pth)
  Dir.glob(pth.join('**').to_s)
end
# }

# { Tasks and Rules definition
CLEAN.include(*subpaths(TMP_PTH), *subpaths(DOXYGEN_CONFIG_FILE_PTH))
CLOBBER.include(*subpaths(TMP_PTH), *subpaths(OBJ_PTH), *subpaths(OUT_PTH))

task :default => [:clobber, :run_tests]
task :full => [:clobber, :static_analysis, :check_mem_leaks, :doc, :run_tests, :run]

task :prepare do
  [SRC_PTH, TEST_PTH, TMP_PTH, OUT_PTH, DOC_PTH, OBJ_PTH].each do |pth|
    FileUtils.mkdir_p(pth) if not pth.directory?
  end
end

task :compile => [:prepare, APP_FILE_PTH]
task :run => [:compile] do
  File.chmod(0555, APP_FILE_PTH) if APP_FILE_PTH.file?
  puts "\n>> App execution Output"
  sh "#{APP_FILE_PTH}"
  puts ">> #{APP_NAME} (pid=#{$?.pid}) exited with status: #{$?.exitstatus}"
end

task :static_analysis do
  puts "\n>> Static analysis execution Output"
  puts `splint +systemdirerrors #{SRC.join(' ')}`
  puts ">> #{APP_NAME} static analysis (pid=#{$?.pid}) exited with status: #{$?.exitstatus}"
end
task :mem_leaks_analysis => [:compile] do
  File.chmod(0555, APP_FILE_PTH) if APP_FILE_PTH.file?
  puts "\n>> Memory leaks analysis execution Output"
  puts `valgrind #{APP_FILE_PTH}`
  puts ">> #{APP_NAME} memory leaks analysis (pid=#{$?.pid}) exited with status: #{$?.exitstatus}"
end

task :doc_prepare => [:prepare] do
  FileUtils.rm DOXYGEN_CONFIG_FILE_PTH if DOXYGEN_CONFIG_FILE_PTH.file?
  config_file_content =  ''
  config_file_content << "PROJECT_NAME = #{APP_NAME}\n"
  config_file_content << "INPUT = #{SRC_PTH}\n"
  config_file_content << "OUTPUT_DIRECTORY = #{DOC_PTH}\n"
  config_file_content << "SOURCE_BROWSER = YES\n"
  config_file_content << "OPTIMIZE_OUTPUT_FOR_C = YES\n"
  config_file_content << "DOXYFILE_ENCODING = UTF-8\n"
  config_file_content << "RECURSIVE = YES\n"
  config_file_content << "EXTRACT_ALL = YES\n"
  config_file_content << "ALPHABETICAL_INDEX = YES\n"
  config_file_content << "ENABLE_PREPROCESSING = YES\n"
  config_file_content << "MACRO_EXPANSION = YES\n"
  config_file_content << "CLASS_DIAGRAMS = YES\n"
  config_file_content << "HAVE_DOT = YES\n"
  config_file_content << "DOT_CLEANUP = YES\n"
  config_file_content << "GRAPHICAL_HIERARCHY = YES\n"
  config_file_content << "GENERATE_LEGEND = YES\n"
  config_file_content << "GENERATE_TESTLIST = YES\n"
  config_file_content << "GENERATE_BUGLIST = YES\n"
  config_file_content << "GENERATE_TODOLIST = YES\n"
  config_file_content << "CLASS_GRAPH = YES\n"
  config_file_content << "COLLABORATION_GRAPH = YES\n"
  config_file_content << "GROUP_GRAPHS = YES\n"
  config_file_content << "INCLUDE_GRAPH = YES\n"
  config_file_content << "INCLUDED_BY_GRAPH = YES\n"
  config_file_content << "CALL_GRAPH = YES\n"
  config_file_content << "CALLER_GRAPH = YES\n"
  config_file_content << "DIRECTORY_GRAPH = YES\n"
  config_file_content << "DOT_IMAGE_FORMAT = png\n"
  config_file_content << "SEARCH_INCLUDES = YES\n"
  config_file_content << "SEARCHENGINE = YES\n"
  if DOC_HTML_ALLOWED
    config_file_content << "GENERATE_HTML = YES\n"
    config_file_content << "HTML_OUTPUT = html/\n"
    config_file_content << "HTML_FILE_EXTENSION = .html\n"
  else
    config_file_content << "GENERATE_HTML = NO\n"
  end
  if DOC_LATEX_ALLOWED
    config_file_content << "GENERATE_LATEX = YES\n"
    config_file_content << "LATEX_OUTPUT = latex/\n"
    config_file_content << "COMPACT_LATEX = YES\n"
    config_file_content << "USE_PDFLATEX = NO\n"
  else
    config_file_content << "GENERATE_LATEX = NO\n"
  end
  if DOC_MAN_ALLOWED
    config_file_content << "GENERATE_MAN = YES\n"
    config_file_content << "MAN_OUTPUT = man/\n"
    config_file_content << "MAN_LINKS = NO\n"
  else
    config_file_content << "GENERATE_MAN = NO\n"
  end
  config_file_content << "\n"
  File.open(DOXYGEN_CONFIG_FILE_PTH, 'w') do |f|
    f.write(config_file_content)
  end
end
task :doc => [:doc_prepare] do
  FileUtils.cd(ROOT_PTH) do
    puts "\n>> Documentation generation Output"
    puts `doxygen #{DOXYGEN_CONFIG_FILE_PTH}`
    puts ">> #{APP_NAME} documentation generation (pid=#{$?.pid}) exited with status: #{$?.exitstatus}"
  end
end

task :compile_tests => [:prepare] do
  sh "cc -o #{TEST_FILE_PTH} #{SRC - [SRC_MAIN_FILE_PTH.to_s] + SRC_TEST} -lcunit"
end
task :run_tests => [:compile_tests] do
  File.chmod(0555, TEST_FILE_PTH) if TEST_FILE_PTH.file?
  puts "\n>> Tests execution Output"
  sh "#{TEST_FILE_PTH}"
  FileUtils.cd(ROOT_PTH) do
    FileUtils.mv Dir.glob('CUnit*'), DOC_PTH.to_s
  end
  puts ">> #{APP_NAME} testing phase (pid=#{$?.pid}) exited with status: #{$?.exitstatus}"
end

# Rule to generate the object files
rule '.o' => lambda{ |obj_file| find_source(obj_file) } do |t|
  sh "cc -c -o #{t.name} #{t.source} -Wall -Wextra --pedantic -std=c99"
end

# Transformation process to get the executable from the OBJ files
file APP_FILE_PTH => OBJ do
  sh "cc -std=c99 -o #{APP_FILE_PTH} #{OBJ}"
end

