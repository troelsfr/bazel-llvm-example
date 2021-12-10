all:
	bazel ${OUTPUT_ARTEFACTS} build --jobs 3 "//..."

debug:
	bazel ${OUTPUT_ARTEFACTS} build --sandbox_debug --spawn_strategy=standalone --save_temps="true" -c dbg --strip never --copt="-g" "//..."

test:
	bazel ${OUTPUT_ARTEFACTS} test --test_output=errors --action_env="GTEST_COLOR=1" "//..."

docker-delete-everything:
	docker rm -vf $(docker ps -a -q)
	docker rmi -f $(docker images -a -q)

coverage:
	## TODO: WiP, see https://medium.com/@naveen.maltesh/generating-code-coverage-report-using-gnu-gcov-lcov-ee54a4de3f11
	## Next step: get bazel to link against gcov
	bazel coverage -s \
		--instrument_test_targets \
		//...     

clean-cache:
	rm -r `bazel info repository_cache`	
	
clean: clean-cache
	bazel clean
	rm -rf bazel-bin/
	rm -rf bazel-wonop/
	rm -rf bazel-genfiles/	
	rm -rf bazel-out/
	rm -rf bazel-testlogs/
	bazel shutdown

