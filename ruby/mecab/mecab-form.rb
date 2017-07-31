#!/usr/local/bin/ruby
# -*- coding: utf-8 -*-
# mecabの出力を使いやすくする

require "open3"
$mecab_path = "/usr/local/bin/mecab"

class Mecab
  attr_reader :result, :with_EOS
  def initialize(input)
    Open3.popen3($mecab_path){|stdin, stdout, stderr, thread|
      stdin.puts(input)
      stdin.close
      stdout.set_encoding("UTF-8")
      @with_EOS = stdout.collect{|item| item}
    }
    @result = @with_EOS.select{|item| item != "EOS\n"}
    @tar = @result
    return self
  end

  #指定された品詞のみを出力する
  def select(pos)    
    @tar = @non_EOS.select{|item| item.split("\t")[1].split(",")[0] == pos}
    return self
  end

  def surf
    @tar.collect{|item| item.split("\t")[0]} 
  end
  
end



p Mecab.new("彼が走る").surf
